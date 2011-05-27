env = Environment()
env['LIBS'] = ['png', 'GL', 'SDL']
env['LIBPATH'] = ['#/libs/SDL/build/.libs/']
env['CPPPATH'] = ['#/libs/SDL/include/']

if env['CC'] == 'gcc':
	env['CFLAGS'].append('-std=c99')

env.Program('spray', Glob('src/*.c'), env)
env.Decider('MD5-timestamp')
