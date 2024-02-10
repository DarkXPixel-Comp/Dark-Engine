using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Generate]
public class Launch : DarkModule
{
	public Launch()
	{
		Name = "Launch";

		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release));

		IsStartup = true;

	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{
		base.Configure(config, target);
		config.AddPublicDependency<Core>(target);
		config.AddPublicDependency<D3D12RHI>(target);
		config.AddPublicDependency<DearImGui>(target);
		config.AddPublicDependency<Input>(target);
	}
}