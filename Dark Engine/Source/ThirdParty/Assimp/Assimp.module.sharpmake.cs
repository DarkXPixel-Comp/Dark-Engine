using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class Assimp : DarkModule
{
	public Assimp()
	{
		Name = "Assimp";

		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release, OutputType.Lib));



	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{
		base.Configure(config, target);
		config.Output = Configuration.OutputType.Utility;
		config.IncludePaths.Add("[project.RootPath]/include");
		config.LibraryPaths.Add("[project.RootPath]/lib/Debug");
		config.LibraryFiles.Add("assimp-vc143-mtd.lib");
		//config.IncludePrivatePaths.Add("[project.RootPath]/private");

	}
}