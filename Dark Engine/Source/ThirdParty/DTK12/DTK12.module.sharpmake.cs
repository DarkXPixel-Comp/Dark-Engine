using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class DTK12 : DarkModule
{
	public DTK12()
	{
		Name = "DTK12";

		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release, OutputType.Lib));



	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{
		base.Configure(config, target);
		config.Output = Configuration.OutputType.Utility;
		config.IncludePaths.Add("[project.RootPath]");
		config.LibraryPaths.Add("[project.RootPath]");
		config.LibraryFiles.Add("DirectXTK12.lib");
		//config.IncludePrivatePaths.Add("[project.RootPath]/private");

	}
}