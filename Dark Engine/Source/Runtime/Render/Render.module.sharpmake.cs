using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Generate]
public class Render : DarkModule
{
	public Render()
	{
		Name = "Render";
		AddTargets(new Target(Platform.win64,
		DevEnv.vs2022, Optimization.Debug | Optimization.Release));

	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{

		base.Configure(config, target);
		config.Output = Configuration.OutputType.Lib;
		config.IncludePaths.Add("[project.RootPath]");
		//config.IncludePrivatePaths.Add("[project.RootPath]/private");

		config.AddPrivateDependency<Core>(target);
		config.AddPrivateDependency<DTK12>(target);
		config.AddPrivateDependency<D3DCompiler>(target);
		config.AddPrivateDependency<Assimp>(target);
		config.AddPublicDependency<DearImGui>(target);
		config.AddPrivateDependency<ApplicationCore>(target);

		config.AddPublicIncludePathModuleName<Engine>(target);



	}
}