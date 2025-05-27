using Flax.Build;
using Flax.Build.NativeCpp;

public class Core : EngineModule
{
	/// <inheritdoc />
	public override void Setup(BuildOptions options)
	{
		base.Setup(options);
		//target_compile_definitions(Core PUBLIC "PLATFORM_HEADER_NAME=Windows")

		options.PublicDefinitions.Add(string.Format("PLATFORM_HEADER_NAME={0}", "Windows"));

		//options.PrivateDependencies.Add("Core");

		//	options.PrivateDefinitions.Add("HELLO");
	}
}
