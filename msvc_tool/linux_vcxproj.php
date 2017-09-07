<?php echo '<?xml version="1.0" encoding="utf-8"?>'."\n";
    $hass_key = CGuidManager::GetRandomString(8) . '-'. CGuidManager::GetRandomString(4) . '-' .
                CGuidManager::GetRandomString(4) . '-'. CGuidManager::GetRandomString(4) . '-' .
                date('YmdHi');
?>

<Project DefaultTargets="Build" ToolsVersion="14.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|ARM">
      <Configuration>Debug</Configuration>
      <Platform>ARM</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|ARM">
      <Configuration>Release</Configuration>
      <Platform>ARM</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Debug|x86">
      <Configuration>Debug</Configuration>
      <Platform>x86</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x86">
      <Configuration>Release</Configuration>
      <Platform>x86</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{<?php echo $hass_key; ?>}</ProjectGuid>
    <Keyword>Linux</Keyword>
    <RootNamespace><?php echo $project; ?></RootNamespace>
    <MinimumVisualStudioVersion>14.0</MinimumVisualStudioVersion>
    <ApplicationType>Linux</ApplicationType>
    <ApplicationTypeRevision>1.0</ApplicationTypeRevision>
    <TargetLinuxPlatform>Generic</TargetLinuxPlatform>
    <LinuxProjectType>{<?php echo $hass_key; ?>}</LinuxProjectType>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|ARM'" Label="Configuration">
    <UseDebugLibraries>true</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|ARM'" Label="Configuration">
    <UseDebugLibraries>false</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x86'" Label="Configuration">
    <UseDebugLibraries>true</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x86'" Label="Configuration">
    <UseDebugLibraries>false</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <UseDebugLibraries>true</UseDebugLibraries>
    <RemoteRootDir>~/projects</RemoteRootDir>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <UseDebugLibraries>false</UseDebugLibraries>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings" />
  <ImportGroup Label="Shared" />
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <OutDir>$(ProjectDir)</OutDir>
    <IntDir>$(ProjectDir)obj\</IntDir>
    <TargetExt />
    <RemoteProjectDir>$(RemoteRootDir)/$(ProjectName)</RemoteProjectDir>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|ARM'">
    <RemoteProjectDir>$(RemoteRootDir)/$(ProjectName)/<?php echo $project; ?></RemoteProjectDir>
  </PropertyGroup>
  <ItemGroup>
<?php foreach ($header as $file) { ?>
    <ClInclude Include="<?php echo $file;?>" />
<?php }?>
  </ItemGroup>
  <ItemGroup>
<?php foreach ($source as $file) { ?>
    <ClCompile Include="<?php echo $file;?>" />
<?php }?>
  </ItemGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <Link>
      <AdditionalDependencies><?php echo $library_debug;?></AdditionalDependencies>
      <LibraryDependencies><?php echo $library_release;?>;%(LibraryDependencies)</LibraryDependencies>
      <AdditionalLibraryDirectories><?php echo $library_dir;?></AdditionalLibraryDirectories>
      <AdditionalOptions><?php echo $link_flag;?></AdditionalOptions>
      <UnresolvedSymbolReferences>是 </UnresolvedSymbolReferences>
      <Relocation>是</Relocation>
      <FunctionBinding>false</FunctionBinding>
      <NoExecStackRequired>false</NoExecStackRequired>
    </Link>
    <ClCompile>
      <ForcedIncludeFiles>%(ForcedIncludeFiles)</ForcedIncludeFiles>
      <PreprocessorDefinitions><?php echo $macros;?>;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <CppLanguageStandard>Default</CppLanguageStandard>
      <CLanguageStandard>Default</CLanguageStandard>
      <AdditionalIncludeDirectories><?php echo $include; ?></AdditionalIncludeDirectories>
      <AdditionalOptions><?php echo $cc_flag;?></AdditionalOptions>
      <CAdditionalWarning />
      <CppAdditionalWarning>
      </CppAdditionalWarning>
      <Optimization>Full</Optimization>
      <StrictAliasing>false</StrictAliasing>
      <OmitFramePointers>true</OmitFramePointers>
      <ExceptionHandling>Enabled</ExceptionHandling>
      <PositionIndependentCode>true</PositionIndependentCode>
      <ObjectFileName>$(IntDir)%(filename).o</ObjectFileName>
      <RuntimeTypeInfo>是</RuntimeTypeInfo>
    </ClCompile>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|ARM'">
    <ClCompile>
      <AdditionalIncludeDirectories><?php echo $include; ?></AdditionalIncludeDirectories>
    </ClCompile>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|ARM'">
    <ClCompile>
      <CAdditionalWarning />
      <PreprocessorDefinitions><?php echo $macros;?>%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <PositionIndependentCode>true</PositionIndependentCode>
      <AdditionalOptions><?php echo $cc_flag;?></AdditionalOptions>
      <CppAdditionalWarning />
      <Optimization>Full</Optimization>
    </ClCompile>
  </ItemDefinitionGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets" />
</Project>