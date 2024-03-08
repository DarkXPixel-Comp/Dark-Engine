using System.IO;
using System.Linq;
using System;
using Sharpmake;
using System.Collections.Generic;


public struct Module
{
	public Type type;
	public string path;

	public Module(Type t, string s)
	{
		this.type = t;
		this.path = s;
	}

}


public static class Modules
{
	public static List<Module> typeModules = new List<Module>();
	
	/*GENERATE_PATH*/ public static string PathSolution = "C:/Users/nahmu/source/repos/Dark Engine";



	/*GENERATE_MODULES*/ static Modules() { typeModules.Add(new Module(typeof(ApplicationCore), "Dark Engine/Source/Runtime/ApplicationCore/")); typeModules.Add(new Module(typeof(Core), "Dark Engine/Source/Runtime/Core/")); typeModules.Add(new Module(typeof(CoreDObject), "Dark Engine/Source/Runtime/CoreDObject/")); typeModules.Add(new Module(typeof(D3D12RHI), "Dark Engine/Source/Runtime/D3D12RHI/")); typeModules.Add(new Module(typeof(Editor), "Dark Engine/Source/Runtime/Editor/")); typeModules.Add(new Module(typeof(Engine), "Dark Engine/Source/Runtime/Engine/")); typeModules.Add(new Module(typeof(Input), "Dark Engine/Source/Runtime/Input/")); typeModules.Add(new Module(typeof(Launch), "Dark Engine/Source/Runtime/Launch/")); typeModules.Add(new Module(typeof(Render), "Dark Engine/Source/Runtime/Render/")); typeModules.Add(new Module(typeof(Renderer), "Dark Engine/Source/Runtime/Renderer/")); typeModules.Add(new Module(typeof(RHI), "Dark Engine/Source/Runtime/RHI/")); typeModules.Add(new Module(typeof(UICore), "Dark Engine/Source/Runtime/UICore/")); typeModules.Add(new Module(typeof(World), "Dark Engine/Source/Runtime/World/")); typeModules.Add(new Module(typeof(Assimp), "Dark Engine/Source/ThirdParty/Assimp/")); typeModules.Add(new Module(typeof(D3DCompiler), "Dark Engine/Source/ThirdParty/D3DCompiler/")); typeModules.Add(new Module(typeof(DearImGui), "Dark Engine/Source/ThirdParty/DearImGui/")); typeModules.Add(new Module(typeof(DTK12), "Dark Engine/Source/ThirdParty/DTK12/"));  }



}





public class DarkModule : Project
{
	//public static List<string> str;
	//public static DarkModule st;

	public bool IsStartup = false;


	public DarkModule()
	{
		Name = "Module";

		SourceRootPath = "";

		//AddTargets(new Target(
		//	Platform.win64,
		//	DevEnv.vs2022,
		//	Optimization.Debug | Optimization.Release, OutputType.Dll));
	}

	public virtual void Configure(Project.Configuration conf, Target target)
	{
		conf.ProjectPath = Path.Combine(Modules.PathSolution, "Dark Engine/Intermediate/ProjectFiles");
		conf.Options.Add(Sharpmake.Options.Vc.General.WindowsTargetPlatformVersion.Latest);
		conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP20);
		conf.TargetPath = Path.Combine(Modules.PathSolution, "Dark Engine", "bin");
		conf.TargetLibraryPath = Path.Combine(Modules.PathSolution, "Dark Engine", "bin", "lib");
		conf.IntermediatePath = Path.Combine(Modules.PathSolution, "Dark Engine/Intermediate/Build/Win64/Sharpmake");
		conf.Output = Configuration.OutputType.Lib;
		conf.ExportAdditionalLibrariesEvenForStaticLib = true;
		conf.DumpDependencyGraph = true;

		if(target.Platform == Platform.win64)
		{
			conf.IncludePaths.Add(Path.Combine(Modules.PathSolution, "Dark Engine/Source/Windows"));

		}
		conf.IncludePaths.Add(Path.Combine(Modules.PathSolution, "Dark Engine/Source"));
	}



}


[Generate]
public class DarkSolution : Solution
{
	List<System.IO.FileInfo> Projects;
	public static List<Type> TypeProjects;


	public DarkSolution()
	{
		Name = "Dark Engine";

		AddTargets(new Target(
			Platform.win64,
			DevEnv.vs2022,
			Optimization.Debug | Optimization.Release, OutputType.Lib));
		
	}

	[Configure]
	public void Configure(Solution.Configuration conf, Target target)
	{
		conf.SolutionPath = @"[solution.SharpmakeCsPath]";
		conf.SolutionFileName = "Dark Engine";


		foreach (var item in Modules.typeModules)
		{
			conf.AddProject(item.type, target, false, item.path);
		}


	}
}




public static class Main
{
	[Sharpmake.Main]
	public static void SharpmakeMain(Sharpmake.Arguments arguments)
	{
		arguments.Generate<DarkSolution>();
	}
}