SamplerState StaticPointWrappedSampler : register(s0, space100);
SamplerState StaticPointClampedSampler : register(s1, space100);
SamplerState StaticBilinearWrappedSampler : register(s2, space100);
SamplerState StaticBilinearClampedSampler : register(s3, space100);
SamplerState StaticTrilinearWrappedSampler : register(s4, space100);
SamplerState StaticTrilinearClampedSampler : register(s5, space100);
SamplerState StaticAnisotropic16WrappedSampler : register(s6, space100);
SamplerState StaticAnisotropic16ClampedSampler : register(s7, space100);

#define GetGlobalSampler(Filter, WrapMode) Static##Filter##WrapMode##Sampler

#define REGISTER_BINDLESS_BUFFER register(b0, space0)


