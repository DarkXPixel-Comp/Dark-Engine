using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class D3D12RHI : DarkModule
{
	public D3D12RHI()
	{
		Name = "D3D12RHI";

		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release, OutputType.Lib));



	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{
		base.Configure(config, target);
		config.Output = Configuration.OutputType.Lib;
		config.IncludePaths.Add("[project.RootPath]/public");
		config.IncludePrivatePaths.Add("[project.RootPath]/private");
		config.AddPrivateDependency<Engine>(target);
		config.AddPrivateDependency<CoreDObject>(target);
		//config.AddPrivateDependency<Engine>(target);
		config.AddPublicDependency<RHI>(target);
		config.AddPublicDependency<Core>(target);

	}
}