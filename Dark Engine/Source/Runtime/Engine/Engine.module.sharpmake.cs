using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class Engine : DarkModule
{
	public Engine()
	{
		Name = "Engine";

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
		config.IncludePaths.Add("[project.RootPath]/Classes");

		config.AddPublicDependency<Core>(target);
		config.AddPublicDependency<UICore>(target);
		config.AddPublicDependency<CoreDObject>(target);
		config.AddPublicDependency<Render>(target);
		config.AddPublicDependency<Editor>(target);


		//config.AddPublicIncludePathModuleName<Input>(target);
		config.AddPublicIncludePathModuleName<RHI>(target);
		config.AddPublicIncludePathModuleName<World>(target);
		//config.AddPublicIncludePathModuleName<UICore>(target);
		//config.AddPublicIncludePathModuleName<RHI>(target);
	}
}