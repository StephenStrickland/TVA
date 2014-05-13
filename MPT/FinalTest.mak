# Microsoft Developer Studio Generated NMAKE File, Based on FinalTest.dsp
!IF "$(CFG)" == ""
CFG=FinalTest - Win32 Debug
!MESSAGE No configuration specified. Defaulting to FinalTest - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "FinalTest - Win32 Release" && "$(CFG)" != "FinalTest - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FinalTest.mak" CFG="FinalTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FinalTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FinalTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "FinalTest - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\FinalTest.exe" "$(OUTDIR)\FinalTest.bsc"


CLEAN :
	-@erase "$(INTDIR)\CameraDS.obj"
	-@erase "$(INTDIR)\CameraDS.sbr"
	-@erase "$(INTDIR)\FinalTest.obj"
	-@erase "$(INTDIR)\FinalTest.pch"
	-@erase "$(INTDIR)\FinalTest.res"
	-@erase "$(INTDIR)\FinalTest.sbr"
	-@erase "$(INTDIR)\FinalTestDlg.obj"
	-@erase "$(INTDIR)\FinalTestDlg.sbr"
	-@erase "$(INTDIR)\MyProc.obj"
	-@erase "$(INTDIR)\MyProc.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\FinalTest.bsc"
	-@erase "$(OUTDIR)\FinalTest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "DirectShow/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FinalTest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\FinalTest.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FinalTest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CameraDS.sbr" \
	"$(INTDIR)\FinalTest.sbr" \
	"$(INTDIR)\FinalTestDlg.sbr" \
	"$(INTDIR)\MyProc.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\FinalTest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=Kernel32.lib Winmm.lib cxcore.lib cv.lib ml.lib cvaux.lib highgui.lib cvcam.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\FinalTest.pdb" /machine:I386 /out:"$(OUTDIR)\FinalTest.exe" /libpath:"DirectShow/Lib" 
LINK32_OBJS= \
	"$(INTDIR)\CameraDS.obj" \
	"$(INTDIR)\FinalTest.obj" \
	"$(INTDIR)\FinalTestDlg.obj" \
	"$(INTDIR)\MyProc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\FinalTest.res"

"$(OUTDIR)\FinalTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FinalTest - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\FinalTest.exe" "$(OUTDIR)\FinalTest.bsc"


CLEAN :
	-@erase "$(INTDIR)\CameraDS.obj"
	-@erase "$(INTDIR)\CameraDS.sbr"
	-@erase "$(INTDIR)\FinalTest.obj"
	-@erase "$(INTDIR)\FinalTest.pch"
	-@erase "$(INTDIR)\FinalTest.res"
	-@erase "$(INTDIR)\FinalTest.sbr"
	-@erase "$(INTDIR)\FinalTestDlg.obj"
	-@erase "$(INTDIR)\FinalTestDlg.sbr"
	-@erase "$(INTDIR)\MyProc.obj"
	-@erase "$(INTDIR)\MyProc.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\FinalTest.bsc"
	-@erase "$(OUTDIR)\FinalTest.exe"
	-@erase "$(OUTDIR)\FinalTest.ilk"
	-@erase "$(OUTDIR)\FinalTest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "DirectShow/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FinalTest.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\FinalTest.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FinalTest.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CameraDS.sbr" \
	"$(INTDIR)\FinalTest.sbr" \
	"$(INTDIR)\FinalTestDlg.sbr" \
	"$(INTDIR)\MyProc.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\FinalTest.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=Kernel32.lib Winmm.lib cxcore.lib cv.lib ml.lib cvaux.lib highgui.lib cvcam.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\FinalTest.pdb" /debug /machine:I386 /out:"$(OUTDIR)\FinalTest.exe" /pdbtype:sept /libpath:"DirectShow/Lib" 
LINK32_OBJS= \
	"$(INTDIR)\CameraDS.obj" \
	"$(INTDIR)\FinalTest.obj" \
	"$(INTDIR)\FinalTestDlg.obj" \
	"$(INTDIR)\MyProc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\FinalTest.res"

"$(OUTDIR)\FinalTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("FinalTest.dep")
!INCLUDE "FinalTest.dep"
!ELSE 
!MESSAGE Warning: cannot find "FinalTest.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "FinalTest - Win32 Release" || "$(CFG)" == "FinalTest - Win32 Debug"
SOURCE=.\CameraDS.cpp

"$(INTDIR)\CameraDS.obj"	"$(INTDIR)\CameraDS.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FinalTest.pch"


SOURCE=.\FinalTest.cpp

"$(INTDIR)\FinalTest.obj"	"$(INTDIR)\FinalTest.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FinalTest.pch"


SOURCE=.\FinalTest.rc

"$(INTDIR)\FinalTest.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\FinalTestDlg.cpp

"$(INTDIR)\FinalTestDlg.obj"	"$(INTDIR)\FinalTestDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FinalTest.pch"


SOURCE=.\MyProc.cpp

"$(INTDIR)\MyProc.obj"	"$(INTDIR)\MyProc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FinalTest.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "FinalTest - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "DirectShow/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FinalTest.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\FinalTest.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "FinalTest - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "DirectShow/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FinalTest.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\FinalTest.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

