##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Enthral
ConfigurationName      :=Debug
WorkspacePath          := "/home/merc/code/enthral-a2"
ProjectPath            := "/home/merc/code/enthral-a2/Enthral"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michae Griffin
Date                   :=04/29/15
CodeLitePath           :="/home/merc/.codelite"
LinkerName             :=/usr/bin/g++ 
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Enthral.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)config++ $(LibrarySwitch)boost_system $(LibrarySwitch)boost_locale $(LibrarySwitch)sqlitewrapped $(LibrarySwitch)sqlite3 
ArLibs                 :=  "config++" "boost_system" "boost_locale" "sqlitewrapped" "sqlite3" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)$(ProjectPath)/src/sqlitewrap 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc 
CXXFLAGS :=  -g -O0 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -std=c++11 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_UserSession.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_StandardIO.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CommonIO.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ConfigIO.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_DatabaseIO.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_UserSession.cpp$(ObjectSuffix): src/UserSession.cpp $(IntermediateDirectory)/src_UserSession.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/enthral-a2/Enthral/src/UserSession.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_UserSession.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_UserSession.cpp$(DependSuffix): src/UserSession.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_UserSession.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_UserSession.cpp$(DependSuffix) -MM "src/UserSession.cpp"

$(IntermediateDirectory)/src_UserSession.cpp$(PreprocessSuffix): src/UserSession.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_UserSession.cpp$(PreprocessSuffix) "src/UserSession.cpp"

$(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix): src/Main.cpp $(IntermediateDirectory)/src_Main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/enthral-a2/Enthral/src/Main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Main.cpp$(DependSuffix): src/Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Main.cpp$(DependSuffix) -MM "src/Main.cpp"

$(IntermediateDirectory)/src_Main.cpp$(PreprocessSuffix): src/Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Main.cpp$(PreprocessSuffix) "src/Main.cpp"

$(IntermediateDirectory)/src_StandardIO.cpp$(ObjectSuffix): src/StandardIO.cpp $(IntermediateDirectory)/src_StandardIO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/enthral-a2/Enthral/src/StandardIO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_StandardIO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_StandardIO.cpp$(DependSuffix): src/StandardIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_StandardIO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_StandardIO.cpp$(DependSuffix) -MM "src/StandardIO.cpp"

$(IntermediateDirectory)/src_StandardIO.cpp$(PreprocessSuffix): src/StandardIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_StandardIO.cpp$(PreprocessSuffix) "src/StandardIO.cpp"

$(IntermediateDirectory)/src_CommonIO.cpp$(ObjectSuffix): src/CommonIO.cpp $(IntermediateDirectory)/src_CommonIO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/enthral-a2/Enthral/src/CommonIO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CommonIO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CommonIO.cpp$(DependSuffix): src/CommonIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CommonIO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CommonIO.cpp$(DependSuffix) -MM "src/CommonIO.cpp"

$(IntermediateDirectory)/src_CommonIO.cpp$(PreprocessSuffix): src/CommonIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CommonIO.cpp$(PreprocessSuffix) "src/CommonIO.cpp"

$(IntermediateDirectory)/src_ConfigIO.cpp$(ObjectSuffix): src/ConfigIO.cpp $(IntermediateDirectory)/src_ConfigIO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/enthral-a2/Enthral/src/ConfigIO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ConfigIO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ConfigIO.cpp$(DependSuffix): src/ConfigIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ConfigIO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ConfigIO.cpp$(DependSuffix) -MM "src/ConfigIO.cpp"

$(IntermediateDirectory)/src_ConfigIO.cpp$(PreprocessSuffix): src/ConfigIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ConfigIO.cpp$(PreprocessSuffix) "src/ConfigIO.cpp"

$(IntermediateDirectory)/src_DatabaseIO.cpp$(ObjectSuffix): src/DatabaseIO.cpp $(IntermediateDirectory)/src_DatabaseIO.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/merc/code/enthral-a2/Enthral/src/DatabaseIO.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_DatabaseIO.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_DatabaseIO.cpp$(DependSuffix): src/DatabaseIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_DatabaseIO.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_DatabaseIO.cpp$(DependSuffix) -MM "src/DatabaseIO.cpp"

$(IntermediateDirectory)/src_DatabaseIO.cpp$(PreprocessSuffix): src/DatabaseIO.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_DatabaseIO.cpp$(PreprocessSuffix) "src/DatabaseIO.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


