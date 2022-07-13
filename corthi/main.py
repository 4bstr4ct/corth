_iota_counter = 0
def _iota(reset=False):
	global _iota_counter

	if reset:
		_iota_counter = 0

	current = _iota_counter
	_iota_counter += 1
	return current


OPERATION_PUSH		= _iota(True)
OPERATION_POP		= _iota()
OPERATION_ADD		= _iota()
OPERATION_SUBTRACT	= _iota()
OPERATION_ECHO		= _iota()
OPERATIONS_COUNT	= _iota()


def _push(operand):
	return (OPERATION_PUSH, operand)


def _pop():
	return (OPERATION_POP, )


def _add():
	return (OPERATION_ADD, )


def _subtract():
	return (OPERATION_ADD, )


def _echo():
	return (OPERATION_ECHO, )


_program = [
	_push(10),
	_push(15),
	_add(),
	_echo()
]


def interpret(program):
	pass


def compile(program):
	pass


def _main():
	compile(_program)


_main()