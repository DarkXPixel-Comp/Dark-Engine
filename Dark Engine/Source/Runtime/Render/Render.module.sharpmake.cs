using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;

[Sharpmake.Compile]
public class Render : DarkModule
{
	public Render()
	{
		Name = "Render";

	}

	[Configure]
	public override void Configure(Project.Configuration config, Target target)
	{

		base.Configure(config, target);
		config.Output = Configuration.OutputType.Lib;
		config.IncludePaths.Add("[project.RootPath]");
		//config.IncludePrivatePaths.Add("[project.RootPath]/private");

		//config.AddPublicDependency<World>(target);


	}
}