:: Copy all dependencies from the y-cruncher directory into the local directory.
:: For developers only. Only run this script if you have the full y-cruncher
:: source code installed.

rd /s /q "source\y-cruncher\ConsoleIO\"
rd /s /q "source\y-cruncher\FileIO\"
rd /s /q "source\y-cruncher\Strings\"
rd /s /q "source\y-cruncher\CVN\"
rd /s /q "source\y-cruncher\DigitViewer\"
rd /s /q "source\y-cruncher\ProcessorCapability\"
del      "source\y-cruncher\Exception.h"
          
xcopy "..\Source Files\Modules_v1.0\Exception.h"               "source\y-cruncher\"
xcopy "..\Source Files\Modules_v1.0\APIs\ConsoleIO"            "source\y-cruncher\ConsoleIO" /s /i
xcopy "..\Source Files\Modules_v1.0\APIs\FileIO"               "source\y-cruncher\FileIO" /s /i
xcopy "..\Source Files\Modules_v1.0\APIs\Strings"              "source\y-cruncher\Strings" /s /i
xcopy "..\Source Files\Modules_v1.0\CVN\ymb_CVN_headers.h"     "source\y-cruncher\CVN\"
xcopy "..\Source Files\Modules_v1.0\CVN\ymb_CVN_u64_forward.h" "source\y-cruncher\CVN\"
xcopy "..\Source Files\Modules_v1.0\CVN\u64_forward"           "source\y-cruncher\CVN\u64_forward" /s /i
xcopy "..\Source Files\y-cruncher\DigitViewer"                 "source\y-cruncher\DigitViewer" /s /i
xcopy "..\Source Files\y-cruncher\ProcessorCapability"         "source\y-cruncher\ProcessorCapability\" /s /i
