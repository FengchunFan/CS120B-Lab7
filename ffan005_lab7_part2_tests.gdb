echo ======================================================\n
echo Running all tests..."\n\n

test "1"
set state = Start
continue 1
expect PORTB 0x01
expect state Action
checkResult

test "2"
set state = Start
continue 2
expect PORTB 0x02
expect state Action
checkResult

test "3"
set state = Start
continue 3
expect PORTB 0x04
expect state Action
checkResult

test "4"
set state = Start
continue 4
expect PORTB 0x02
expect state Action
checkResult

test "Pause"
setPINA ~0x01
continue 1
expect state Pause
expect PORTB 0x02
checkResult

test "Resume"
setPINA ~0x01
continue 1
setPINA ~0x00
continue 1
expect state Action
expect PORTB 0x04
checkResult

set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
