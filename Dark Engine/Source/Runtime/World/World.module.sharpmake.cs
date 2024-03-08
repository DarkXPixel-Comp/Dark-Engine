using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class World : DarkModule
{
	public World()
	{
		Name = "World";

		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release, OutputType.Lib));



	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{
		base.Configure(config, target);
		//config.Output = Configuration.OutputType.Lib;
		config.IncludePaths.Add("[project.RootPath]");
		//config.IncludePrivatePaths.Add("[project.RootPath]/private");
		config.AddPrivateDependency<Core>(target);
		config.AddPrivateDependency<Engine>(target);

	}
}