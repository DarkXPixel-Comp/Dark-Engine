using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class ApplicationCore : DarkModule
{
	public ApplicationCore()
	{
		Name = "ApplicationCore";

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


		config.AddPrivateDependency<DTK12>(target, DependencySetting.DefaultWithoutLinking);
		config.AddPublicDependency<Core>(target);
		config.AddPublicDependency<Input>(target);
		config.AddPrivateDependency<DearImGui>(target);
		config.AddPublicIncludePathModuleName<RHI>(target);


	}
}