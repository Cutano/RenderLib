using System.Diagnostics;

namespace RenderCore.FrameGraph;

public class Compiler
{
    public class Result
    {
        public class ResourceInfo
        {
            /// <summary>
            /// Pass that writes to this resource.
            /// </summary>
            public PassNode? WriterPass { get; internal set; }
            /// <summary>
            /// Pass that copy to this resource.
            /// </summary>
            public PassNode? CopyPass { get; internal set; }

            /// <summary>
            /// Passes that read this resource.
            /// </summary>
            public List<PassNode> ReaderPasses { get; internal set; } = new();

            public PassNode? FirstReferencedPass { get; internal set; }
            public PassNode? LastReferencedPass { get; internal set; }
            public int ReferenceCount { get; internal set; }
        }
        
        public class PassInfo
        {
            public List<ResourceNode> ConstructedResource { get; internal set; } = new();
            public List<ResourceNode> DisposedResource { get; internal set; } = new();
            public List<ResourceNode> MovedResource { get; internal set; } = new();
        }
        
        public class PassGraph
        {
            public Dictionary<PassNode, HashSet<PassNode>> AdjList { get; internal set; } = new();

            public void Clear() => AdjList.Clear();

            public List<PassNode>? Sort()
            {
                var inDegMap = AdjList.ToDictionary(pair => pair.Key, _ => 0);

                foreach (var pass in AdjList.SelectMany(pair => pair.Value))
                {
                    inDegMap[pass]++;
                }

                var zeroInDegNodes = new Stack<PassNode>();
                var sortedNodes = new List<PassNode>();

                foreach (var pair in inDegMap.Where(pair => pair.Value == 0))
                {
                    zeroInDegNodes.Push(pair.Key);
                }

                while (zeroInDegNodes.Count != 0)
                {
                    var node = zeroInDegNodes.Pop();
                    sortedNodes.Add(node);
                    inDegMap.Remove(node);

                    foreach (var pass in AdjList[node].Where(pass => inDegMap.ContainsKey(pass)))
                    {
                        inDegMap[pass]--;
                        if (inDegMap[pass] == 0)
                        {
                            zeroInDegNodes.Push(pass);
                        }
                    }
                }

                return inDegMap.Count != 0 ? null : sortedNodes;
            }

            public void ToGraphVis()
            {
                
            }
        }

        public PassGraph? CompiledGraph { get; internal set; }
        public List<PassNode>? SortedPasses { get; internal set; }
        public Dictionary<PassNode, int> PassToOrder { get; internal set; } = new();
        public Dictionary<ResourceNode, ResourceInfo> ResourceInfoRegistry { get; internal set; } = new();
        public Dictionary<PassNode, PassInfo> PassInfoRegistry { get; internal set; } = new();
        public Dictionary<ResourceNode, ResourceNode> MoveSourceToDestination { get; internal set; } = new();
        public Dictionary<ResourceNode, ResourceNode> MoveDestinationToSource { get; internal set; } = new();
        public Dictionary<ResourceNode, ResourceNode> CopySourceToDestination { get; internal set; } = new();
        public Dictionary<ResourceNode, ResourceNode> CopyDestinationToSource { get; internal set; } = new();
    }

    public Result Compile(FrameGraph frameGraph)
    {
        var result = new Result();

        foreach (var pass in frameGraph.PassNodes)
        {
            switch (pass.PassType)
            {
                case PassNode.Type.General:
                    foreach (var input in pass.Inputs)
                    {
                        if (!result.ResourceInfoRegistry.ContainsKey(input))
                        {
                            result.ResourceInfoRegistry[input] = new Result.ResourceInfo();
                        }
                        
                        result.ResourceInfoRegistry[input].ReaderPasses.Add(pass);
                    }
                    
                    foreach (var output in pass.Outputs)
                    {
                        if (!result.ResourceInfoRegistry.ContainsKey(output))
                        {
                            result.ResourceInfoRegistry[output] = new Result.ResourceInfo();
                        }

                        if (result.ResourceInfoRegistry[output].WriterPass is not null)
                        {
                            throw new Exception($"Multiple WriterPass in {output.Name}");
                        }

                        result.ResourceInfoRegistry[output].WriterPass = pass;
                    }
                    break;
                case PassNode.Type.Copy:
                    foreach (var input in pass.Inputs)
                    {
                        if (!result.ResourceInfoRegistry.ContainsKey(input))
                        {
                            result.ResourceInfoRegistry[input] = new Result.ResourceInfo();
                        }
                        
                        result.ResourceInfoRegistry[input].ReaderPasses.Add(pass);
                    }

                    foreach (var output in pass.Outputs)
                    {
                        if (!result.ResourceInfoRegistry.ContainsKey(output))
                        {
                            result.ResourceInfoRegistry[output] = new Result.ResourceInfo();
                        }
                        
                        if (result.ResourceInfoRegistry[output].CopyPass is not null)
                        {
                            throw new Exception($"Multiple CopyPass in {output.Name}");
                        }
                        
                        result.ResourceInfoRegistry[output].CopyPass = pass;
                    }
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
        
        foreach (var moveNode in frameGraph.MoveNodes)
        {
            if (result.MoveSourceToDestination.ContainsKey(moveNode.Source))
            {
                throw new Exception($"Moved out more than once in {moveNode.Name}");
            }

            if (result.MoveSourceToDestination.ContainsValue(moveNode.Destination))
            {
                throw new Exception($"Moved in more than once in {moveNode.Name}");
            }
            
            result.MoveSourceToDestination.Add(moveNode.Source, moveNode.Destination);
        }

        foreach (var copyPass in frameGraph.PassNodes.Where(passNode => passNode.PassType == PassNode.Type.Copy))
        {
            if (copyPass.Inputs.Count > 1)
            {
                throw new Exception($"Multiple copy source {copyPass.Name}");
            }

            if (copyPass.Outputs.Count > 1)
            {
                throw new Exception($"Multiple copy destination {copyPass.Name}");
            }
            
            result.CopySourceToDestination.Add(copyPass.Inputs[0], copyPass.Outputs[0]);
        }

        var moveNodesToRemove = new HashSet<ResourceNode>();
        foreach (var (src, dest) in result.MoveSourceToDestination)
        {
            if (moveNodesToRemove.Contains(src) || !result.MoveSourceToDestination.ContainsKey(dest))
            {
                continue;
            }

            var next = result.MoveSourceToDestination[dest];
            var info = result.ResourceInfoRegistry[dest];
            while (info.WriterPass == null && info.ReaderPasses.Count == 0)
            {
                result.MoveSourceToDestination[src] = next;
                info = result.ResourceInfoRegistry[next];
                moveNodesToRemove.Add(next);
                if (!result.MoveSourceToDestination.ContainsKey(next))
                {
                    break;
                }
                
                next = result.MoveSourceToDestination[next];
            }
        }

        foreach (var node in moveNodesToRemove)
        {
            result.CopySourceToDestination.Remove(node);
        }

        if (frameGraph.PassNodes.Count != 0)
        {
            result.CompiledGraph = new Result.PassGraph();
        }
        
        foreach (var passNode in frameGraph.PassNodes)
        {
            result.CompiledGraph?.AdjList.Add(passNode, new HashSet<PassNode>());
        }

        foreach (var info in result.ResourceInfoRegistry)
        {
            if (info.Value.WriterPass is not null)
            {
                foreach (var readerPass in info.Value.ReaderPasses)
                {
                    result.CompiledGraph?.AdjList[info.Value.WriterPass].Add(readerPass);
                }
            }

            if (info.Value.CopyPass is not null)
            {
                foreach (var readerPass in info.Value.ReaderPasses)
                {
                    result.CompiledGraph?.AdjList[info.Value.CopyPass].Add(readerPass);
                }
            }

            if (info.Value.WriterPass is not null &&
                info.Value.CopyPass is not null &&
                info.Value.ReaderPasses.Count == 0)
            {
                result.CompiledGraph?.AdjList[info.Value.WriterPass].Add(info.Value.CopyPass);
            }
        }

        foreach (var (src, dst) in result.MoveSourceToDestination)
        {
            var infoDst = result.ResourceInfoRegistry[dst];
            var infoSrc = result.ResourceInfoRegistry[src];
            
            var firstAccessoriesDst = new List<PassNode>();
            var finalAccessoriesSrc = new List<PassNode>();
            
            if (infoDst.WriterPass is not null)
            {
                firstAccessoriesDst = new List<PassNode>
                {
                    infoDst.WriterPass
                };
            }
            else if (infoDst.ReaderPasses.Count != 0)
            {
                firstAccessoriesDst = infoDst.ReaderPasses;
            }
            else if (infoDst.CopyPass is not null)
            {
                firstAccessoriesDst = new List<PassNode>
                {
                    infoDst.CopyPass
                };
            }
            
            if (infoSrc.WriterPass is not null)
            {
                finalAccessoriesSrc = new List<PassNode>
                {
                    infoSrc.WriterPass
                };
            }
            else if (infoSrc.ReaderPasses.Count != 0)
            {
                finalAccessoriesSrc = infoSrc.ReaderPasses;
            }
            else if (infoSrc.CopyPass is not null)
            {
                finalAccessoriesSrc = new List<PassNode>
                {
                    infoSrc.CopyPass
                };
            }

            foreach (var finalAccessor in finalAccessoriesSrc)
            {
                foreach (var firstAccessor in firstAccessoriesDst)
                {
                    result.CompiledGraph?.AdjList[finalAccessor].Add(firstAccessor);
                }
            }
        }

        var sortedPass = result.CompiledGraph?.Sort();
        if (sortedPass is null)
        {
            throw new Exception("Not a DAG");
        }

        result.SortedPasses = sortedPass;

        foreach (var (src, dst) in result.MoveSourceToDestination)
        {
            result.MoveDestinationToSource.Add(dst, src);
        }

        foreach (var (src, dst) in result.CopySourceToDestination)
        {
            result.CopyDestinationToSource.Add(dst, src);
        }

        for (var i = 0; i < result.SortedPasses.Count; i++)
        {
            result.PassToOrder[result.SortedPasses[i]] = i;
        }

        foreach (var node in frameGraph.PassNodes)
        {
            result.PassInfoRegistry[node] = new Result.PassInfo();
        }

        foreach (var pair in result.ResourceInfoRegistry)
        {
            var info = pair.Value;

            var passToRemove = 
                info.ReaderPasses.Where(readerPass => !result.SortedPasses.Contains(readerPass)).ToList();
            foreach (var passNode in passToRemove)
            {
                info.ReaderPasses.Remove(passNode);
            }

            info.ReferenceCount = info.ReaderPasses.Count;

            if (info.WriterPass is not null)
            {
                info.FirstReferencedPass = info.WriterPass;
            }
            else if (info.ReaderPasses.Count != 0)
            {
                info.FirstReferencedPass = result.SortedPasses[
                    info.ReaderPasses.Select(readerPass => result.PassToOrder[readerPass]).Min()];
            }
            else if (info.CopyPass is not null)
            {
                info.FirstReferencedPass = info.CopyPass;
            }

            info.LastReferencedPass = info.FirstReferencedPass;
            if (info.CopyPass is not null)
            {
                info.LastReferencedPass = info.CopyPass;
            }
            else if (info.ReaderPasses.Count != 0)
            {
                info.LastReferencedPass = result.SortedPasses[
                    info.ReaderPasses.Select(readerPass => result.PassToOrder[readerPass]).Max()];
            }
        }

        foreach (var (resourceNode, info) in result.ResourceInfoRegistry)
        {
            if (info.FirstReferencedPass is null && result.MoveDestinationToSource.ContainsKey(resourceNode))
            {
                var target = result.MoveDestinationToSource[resourceNode];
                info.FirstReferencedPass = result.ResourceInfoRegistry[target].LastReferencedPass;
                info.LastReferencedPass ??= info.FirstReferencedPass;
            }
            
            Debug.Assert(info.FirstReferencedPass is not null);
            Debug.Assert(info.LastReferencedPass is not null);
            
            if (result.PassToOrder[info.LastReferencedPass] < result.PassToOrder[info.FirstReferencedPass])
            {
                info.LastReferencedPass = info.FirstReferencedPass;
            }
        }

        foreach (var (resourceNode, info) in result.ResourceInfoRegistry)
        {
            Debug.Assert(info.FirstReferencedPass is not null);
            Debug.Assert(info.LastReferencedPass is not null);

            if (!result.MoveDestinationToSource.ContainsKey(resourceNode))
            {
                result.PassInfoRegistry[info.FirstReferencedPass].ConstructedResource.Add(resourceNode);
            }

            if (result.MoveSourceToDestination.ContainsKey(resourceNode))
            {
                result.PassInfoRegistry[info.LastReferencedPass].MovedResource.Add(resourceNode);
            }
            else
            {
                if (info.ReaderPasses.Count > 1 && info.ReaderPasses.Any(readerPass => readerPass.Async))
                {
                    var lastRefIndex = result.PassToOrder[info.LastReferencedPass];
                    foreach (var passNode in result.SortedPasses.Where(
                                 passNode => passNode.Async == false && result.PassToOrder[passNode] >= lastRefIndex))
                    {
                        result.PassInfoRegistry[passNode].DisposedResource.Add(resourceNode);
                    }
                }
                else
                {
                    result.PassInfoRegistry[info.LastReferencedPass].DisposedResource.Add(resourceNode);
                }
            }
        }

        return result;
    }
}