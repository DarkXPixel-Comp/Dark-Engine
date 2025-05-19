using Flax.Build;
using Flax.Build.NativeCpp;

public class Launch : EngineModule
{
	/// <inheritdoc />
	public override void Setup(BuildOptions options)
	{
		base.Setup(options);

		options.PrivateDependencies.Add("Core");

		//options.PrivateDefinitions.Add("HELLO");
	}
}
