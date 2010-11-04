import utility
import dependency
import imp_module
import scons_tools
from SCons.Script import Builder, File, Action, Glob, Return, Alias, Dir


def IMPCPPApplication(envi, target, source, required_modules=[],
                      optional_dependencies=[],
                      required_dependencies=[]):
    if envi.GetOption('help'):
        return
    for m in required_modules:
        if not envi.get_module_ok(m):
            print target, "disabled due to missing module IMP."+m
            return
    for d in required_dependencies:
        if not envi.get_dependency_ok(d):
            print target, "disabled due to missing dependency", d
            return
    print "Configuration application", target
    if len(required_modules+required_dependencies)>0:
        print "  (requires", ", ".join(required_modules+required_dependencies)+")"
    env= scons_tools.get_bin_environment(envi)

    utility.add_link_flags(env, required_modules,
                           required_dependencies+env.get_found_dependencies(optional_dependencies))

    prog= env.Program(target=target, source=source)
    bindir = env.GetInstallDirectory('bindir')
    build= env.Install("#/build/bin", prog)
    env['IMP_APPLICATION']=str(target)
    install = env.Install(bindir, prog)

    env.SConscript('test/SConscript', exports=['env'])
    env.Alias(env['IMP_APPLICATION'], build)
    env.Alias(env['IMP_APPLICATION']+"-install", install)
    env.Alias("all", build)
    env.Alias("install", install)
def IMPApplicationTest(env, python_tests=[]):
    files= ["#/tools/imppy.sh", "#/scons_tools/run-all-tests.py"]+\
        [File(x).abspath for x in python_tests]
    test = env.IMPApplicationRunTest(target="test.passed", source=files,
                                     TEST_TYPE='unit test')
    env.AlwaysBuild("test.passed")
    env.Requires(test, env.Alias(env['IMP_APPLICATION']))
    env.Alias('test', test)
