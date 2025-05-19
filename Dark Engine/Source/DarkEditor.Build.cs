using System.IO;
using Flax.Build;
using Flax.Build.NativeCpp;


public class DarkEditor : EngineTarget
{
	public override void Init()
	{
		base.Init();

		IsEditor = true;
		OutputName = "DarkEditor";
		ConfigurationName = "Editor";
		IsPreBuilt = false;
		Platforms = new[]
		{
			TargetPlatform.Windows
		};
		Architectures = new[]
		{
			TargetArchitecture.x64
		};
		GlobalDefinitions.Add("USE_EDITOR");
	}

	public override void SetupTargetEnvironment(BuildOptions options)
	{
		base.SetupTargetEnvironment(options);

		// Setup output folder for Editor binaries
		switch (options.Platform.Target)
		{
			case TargetPlatform.Windows:
				switch (options.Architecture)
				{
					case TargetArchitecture.x64:
						options.OutputFolder = Path.Combine(options.WorkingDirectory, "Binaries", "Editor", "Win64", options.Configuration.ToString());
						break;
					default: throw new InvalidArchitectureException(options.Architecture, "Not supported Editor architecture.");
				}
				break;
			default: throw new InvalidPlatformException(options.Platform.Target, "Not supported Editor platform.");
		}
	}
}