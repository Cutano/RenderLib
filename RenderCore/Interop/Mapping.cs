namespace RenderCore.Interop;

public partial struct DeviceObjectAttribs
{
    public string Name;
    
    [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, Pack = 0, CharSet = System.Runtime.InteropServices.CharSet.Unicode)]
    internal partial struct __Native
    {
        public System.IntPtr Name;
    }
    
    internal unsafe void __MarshalFree(ref __Native @ref)
    {
        System.Runtime.InteropServices.Marshal.FreeHGlobal(@ref.Name);
    }
    
    internal unsafe void __MarshalFrom(ref __Native @ref)
    {
        Name = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(@ref.Name) ?? string.Empty;
    }
    
    internal unsafe void __MarshalTo(ref __Native @ref)
    {
        @ref.Name = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(Name);
    }
}

public partial struct ShaderCreateInfo
{
    public string FilePath;
    public RenderCore.Interop.IShaderSourceInputStreamFactory ShaderSourceStreamFactory;
    public RenderCore.Interop.IHLSL2GLSLConversionStream PpConversionStream;
    public string Source;
    public System.IntPtr ByteCode;
    public long SourceLength;
    public long ByteCodeSize;
    
    [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, Pack = 0, CharSet = System.Runtime.InteropServices.CharSet.Unicode)]
    internal partial struct __Native
    {
        public System.IntPtr FilePath;
        public System.IntPtr ShaderSourceStreamFactory;
        public System.IntPtr PpConversionStream;
        public System.IntPtr Source;
        public System.IntPtr ByteCode;
        // public long SourceLength;
        // public long ByteCodeSize;
        public long SourceLengthByteCodeSize;
        public System.IntPtr EntryPoint;
        public System.IntPtr Macros;
        public RenderCore.Interop.ShaderDesc.__Native Desc;
        public RenderCore.Interop.ShaderSourceLanguage SourceLanguage;
        public RenderCore.Interop.ShaderCompiler ShaderCompiler;
        public RenderCore.Interop.Version HLSLVersion;
        public RenderCore.Interop.Version GLSLVersion;
        public RenderCore.Interop.Version GLESSLVersion;
        public RenderCore.Interop.Version MSLVersion;
        public RenderCore.Interop.ShaderCompileFlags CompileFlags;
        public System.IntPtr PpCompilerOutput;
    }
    
    internal unsafe void __MarshalFree(ref __Native @ref)
    {
        System.Runtime.InteropServices.Marshal.FreeHGlobal(@ref.FilePath);
        System.Runtime.InteropServices.Marshal.FreeHGlobal(@ref.Source);
        System.Runtime.InteropServices.Marshal.FreeHGlobal(@ref.EntryPoint);
        Desc.__MarshalFree(ref @ref.Desc);
        System.GC.KeepAlive(ShaderSourceStreamFactory);
        System.GC.KeepAlive(PpConversionStream);
        System.GC.KeepAlive(ByteCode);
        System.GC.KeepAlive(Macros);
        System.GC.KeepAlive(PpCompilerOutput);
    }
    
    internal unsafe void __MarshalFrom(ref __Native @ref)
    {
        FilePath = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(@ref.FilePath) ?? string.Empty;
        ShaderSourceStreamFactory = @ref.ShaderSourceStreamFactory != System.IntPtr.Zero
            ? new RenderCore.Interop.IShaderSourceInputStreamFactory(@ref.ShaderSourceStreamFactory)
            : null;
        PpConversionStream = @ref.PpConversionStream != System.IntPtr.Zero
            ? new RenderCore.Interop.IHLSL2GLSLConversionStream(@ref.PpConversionStream)
            : null;
        Source = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(@ref.Source);
        ByteCode = @ref.ByteCode;
        SourceLength = @ref.SourceLengthByteCodeSize;
        ByteCodeSize = @ref.SourceLengthByteCodeSize;
        EntryPoint = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(@ref.EntryPoint);
        Macros = @ref.Macros;
        Desc.__MarshalFrom(ref @ref.Desc);
        SourceLanguage = @ref.SourceLanguage;
        ShaderCompiler = @ref.ShaderCompiler;
        HLSLVersion = @ref.HLSLVersion;
        GLSLVersion = @ref.GLSLVersion;
        GLESSLVersion = @ref.GLESSLVersion;
        MSLVersion = @ref.MSLVersion;
        CompileFlags = @ref.CompileFlags;
        PpCompilerOutput = @ref.PpCompilerOutput != System.IntPtr.Zero
            ? new RenderCore.Interop.IDataBlob(@ref.PpCompilerOutput)
            : null;
    }
    
    internal unsafe void __MarshalTo(ref __Native @ref)
    {
        @ref.FilePath = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(FilePath);
        @ref.ShaderSourceStreamFactory = ShaderSourceStreamFactory?.NativePointer ?? System.IntPtr.Zero;
        @ref.PpConversionStream = PpConversionStream?.NativePointer ?? System.IntPtr.Zero;
        @ref.Source = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(Source);
        @ref.ByteCode = ByteCode;
        @ref.SourceLengthByteCodeSize = SourceLength + ByteCodeSize;
        @ref.EntryPoint = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(EntryPoint);
        @ref.Macros = Macros;
        Desc.__MarshalTo(ref @ref.Desc);
        @ref.SourceLanguage = SourceLanguage;
        @ref.ShaderCompiler = ShaderCompiler;
        @ref.HLSLVersion = HLSLVersion;
        @ref.GLSLVersion = GLSLVersion;
        @ref.GLESSLVersion = GLESSLVersion;
        @ref.MSLVersion = MSLVersion;
        @ref.CompileFlags = CompileFlags;
        @ref.PpCompilerOutput = PpCompilerOutput?.NativePointer ?? System.IntPtr.Zero;
    }
}

public partial struct GraphicsAdapterInfo
{
    /// <unmanaged>GraphicsAdapterInfo::Description</unmanaged>
    /// <unmanaged-short>Description</unmanaged-short>
    public string Description;

    /// <unmanaged>GraphicsAdapterInfo::Type</unmanaged>
    /// <unmanaged-short>Type</unmanaged-short>
    public RenderCore.Interop.AdapterType Type;

    /// <unmanaged>GraphicsAdapterInfo::Vendor</unmanaged>
    /// <unmanaged-short>Vendor</unmanaged-short>
    public RenderCore.Interop.AdapterVendor Vendor;

    /// <unmanaged>GraphicsAdapterInfo::VendorId</unmanaged>
    /// <unmanaged-short>VendorId</unmanaged-short>
    public uint VendorId;

    /// <unmanaged>GraphicsAdapterInfo::DeviceId</unmanaged>
    /// <unmanaged-short>DeviceId</unmanaged-short>
    public uint DeviceId;

    /// <unmanaged>GraphicsAdapterInfo::NumOutputs</unmanaged>
    /// <unmanaged-short>NumOutputs</unmanaged-short>
    public uint NumOutputs;

    /// <unmanaged>GraphicsAdapterInfo::Memory</unmanaged>
    /// <unmanaged-short>Memory</unmanaged-short>
    public RenderCore.Interop.AdapterMemoryInfo Memory;

    /// <unmanaged>GraphicsAdapterInfo::RayTracing</unmanaged>
    /// <unmanaged-short>RayTracing</unmanaged-short>
    public RenderCore.Interop.RayTracingProperties RayTracing;

    /// <unmanaged>GraphicsAdapterInfo::WaveOp</unmanaged>
    /// <unmanaged-short>WaveOp</unmanaged-short>
    public RenderCore.Interop.WaveOpProperties WaveOp;

    /// <unmanaged>GraphicsAdapterInfo::Buffer</unmanaged>
    /// <unmanaged-short>Buffer</unmanaged-short>
    public RenderCore.Interop.BufferProperties Buffer;

    /// <unmanaged>GraphicsAdapterInfo::Texture</unmanaged>
    /// <unmanaged-short>Texture</unmanaged-short>
    public RenderCore.Interop.TextureProperties Texture;

    /// <unmanaged>GraphicsAdapterInfo::Sampler</unmanaged>
    /// <unmanaged-short>Sampler</unmanaged-short>
    public RenderCore.Interop.SamplerProperties Sampler;

    /// <unmanaged>GraphicsAdapterInfo::MeshShader</unmanaged>
    /// <unmanaged-short>MeshShader</unmanaged-short>
    public RenderCore.Interop.MeshShaderProperties MeshShader;

    /// <unmanaged>GraphicsAdapterInfo::ShadingRate</unmanaged>
    /// <unmanaged-short>ShadingRate</unmanaged-short>
    public RenderCore.Interop.ShadingRateProperties ShadingRate;

    /// <unmanaged>GraphicsAdapterInfo::ComputeShader</unmanaged>
    /// <unmanaged-short>ComputeShader</unmanaged-short>
    public RenderCore.Interop.ComputeShaderProperties ComputeShader;

    /// <unmanaged>GraphicsAdapterInfo::DrawCommand</unmanaged>
    /// <unmanaged-short>DrawCommand</unmanaged-short>
    public RenderCore.Interop.DrawCommandProperties DrawCommand;

    /// <unmanaged>GraphicsAdapterInfo::SparseResources</unmanaged>
    /// <unmanaged-short>SparseResources</unmanaged-short>
    public RenderCore.Interop.SparseResourceProperties SparseResources;

    /// <unmanaged>GraphicsAdapterInfo::Features</unmanaged>
    /// <unmanaged-short>Features</unmanaged-short>
    public RenderCore.Interop.DeviceFeatures Features;

    internal RenderCore.Interop.CommandQueueInfo[] _Queues;

    /// <unmanaged>GraphicsAdapterInfo::Queues</unmanaged>
    /// <unmanaged-short>Queues</unmanaged-short>
    public RenderCore.Interop.CommandQueueInfo[] Queues
    {
        get => _Queues ??= new RenderCore.Interop.CommandQueueInfo[16];
        set => this._Queues = value;
    }

    /// <unmanaged>GraphicsAdapterInfo::NumQueues</unmanaged>
    /// <unmanaged-short>NumQueues</unmanaged-short>
    public uint NumQueues;

    [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential,
        Pack = 0, CharSet = System.Runtime.InteropServices.CharSet.Unicode)]
    internal unsafe partial struct __Native
    {
        public fixed byte Description[128];
        public RenderCore.Interop.AdapterType Type;
        public RenderCore.Interop.AdapterVendor Vendor;
        public uint VendorId;
        public uint DeviceId;
        public uint NumOutputs;
        public RenderCore.Interop.AdapterMemoryInfo Memory;
        public RenderCore.Interop.RayTracingProperties RayTracing;
        public RenderCore.Interop.WaveOpProperties WaveOp;
        public RenderCore.Interop.BufferProperties Buffer;
        public RenderCore.Interop.TextureProperties Texture;
        public RenderCore.Interop.SamplerProperties Sampler;
        public RenderCore.Interop.MeshShaderProperties MeshShader;
        public RenderCore.Interop.ShadingRateProperties.__Native ShadingRate;
        public RenderCore.Interop.ComputeShaderProperties ComputeShader;
        public RenderCore.Interop.DrawCommandProperties DrawCommand;
        public RenderCore.Interop.SparseResourceProperties SparseResources;
        public RenderCore.Interop.DeviceFeatures Features;
        public fixed byte Queues[16 * 12];
        public uint NumQueues;
    }

    internal unsafe void __MarshalFree(ref __Native @ref)
    {
        ShadingRate.__MarshalFree(ref @ref.ShadingRate);
    }

    internal unsafe void __MarshalFrom(ref __Native @ref)
    {
        fixed (void* __ptr = @ref.Description)
            Description = SharpGen.Runtime.StringHelpers.PtrToStringAnsi((System.IntPtr)__ptr, 127);
        Type = @ref.Type;
        Vendor = @ref.Vendor;
        VendorId = @ref.VendorId;
        DeviceId = @ref.DeviceId;
        NumOutputs = @ref.NumOutputs;
        Memory = @ref.Memory;
        RayTracing = @ref.RayTracing;
        WaveOp = @ref.WaveOp;
        Buffer = @ref.Buffer;
        Texture = @ref.Texture;
        Sampler = @ref.Sampler;
        MeshShader = @ref.MeshShader;
        ShadingRate.__MarshalFrom(ref @ref.ShadingRate);
        ComputeShader = @ref.ComputeShader;
        DrawCommand = @ref.DrawCommand;
        SparseResources = @ref.SparseResources;
        Features = @ref.Features;

        // for (int i = 0, __length = Queues.Length; i < __length; ++i)
        //     Queues[i].__MarshalFrom(ref @ref.Queues[i * 12]);
        
        NumQueues = @ref.NumQueues;
    }

    internal unsafe void __MarshalTo(ref __Native @ref)
    {
        {
            int Description_length = System.Math.Min(Description?.Length ?? 0, 127);
            System.IntPtr __from = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(Description);
            fixed (byte* __to = @ref.Description)
            {
                SharpGen.Runtime.MemoryHelpers.CopyMemory((System.IntPtr)__to, __from, Description_length);
                __to[Description_length] = 0;
            }

            System.Runtime.InteropServices.Marshal.FreeHGlobal(__from);
        }

        @ref.Type = Type;
        @ref.Vendor = Vendor;
        @ref.VendorId = VendorId;
        @ref.DeviceId = DeviceId;
        @ref.NumOutputs = NumOutputs;
        @ref.Memory = Memory;
        @ref.RayTracing = RayTracing;
        @ref.WaveOp = WaveOp;
        @ref.Buffer = Buffer;
        @ref.Texture = Texture;
        @ref.Sampler = Sampler;
        @ref.MeshShader = MeshShader;
        ShadingRate.__MarshalTo(ref @ref.ShadingRate);
        @ref.ComputeShader = ComputeShader;
        @ref.DrawCommand = DrawCommand;
        @ref.SparseResources = SparseResources;
        @ref.Features = Features;
        
        // for (int i = 0, __length = Queues.Length; i < __length; ++i)
        //     Queues[i].__MarshalTo(ref @ref.Queues[i * 12]);
        
        @ref.NumQueues = NumQueues;
    }
}