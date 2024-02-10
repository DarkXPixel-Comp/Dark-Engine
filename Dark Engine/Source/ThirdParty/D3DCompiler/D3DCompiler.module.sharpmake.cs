using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class D3DCompiler : DarkModule
{
	public D3DCompiler()
	{
		Name = "D3DCompiler";

		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release, OutputType.Lib));



	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{
		base.Configure(config, target);
		config.Output = Configuration.OutputType.Lib;
		config.IncludePaths.Add("[project.RootPath]/inc");
		//config.IncludePrivatePaths.Add("[project.RootPath]/private");

	}
}