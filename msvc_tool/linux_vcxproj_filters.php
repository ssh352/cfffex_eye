<?php echo '<?xml version="1.0" encoding="utf-8"?>'."\n"; ?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
    <ItemGroup>
<?php foreach ($filters as $filtername => $v) {
            $hass_key = CGuidManager::GetRandomString(8) . '-'. CGuidManager::GetRandomString(4) . '-' .
                                    CGuidManager::GetRandomString(4) . '-'. CGuidManager::GetRandomString(4) . '-' .
                                    date('YmdHi');
?>
        <Filter Include="<?php echo $filtername;?>">
            <UniqueIdentifier>{<?php echo $hass_key; ?>}</UniqueIdentifier>
        </Filter>
<?php }?>
    </ItemGroup>

<?php foreach ($filters as $filtername => $files) {?>
<?php if (isset($files['header'])) { ?>
    <ItemGroup>
<?php foreach ($files['header'] as $file) { ?>
        <ClInclude Include="<?php echo $file;?>">
            <Filter><?php echo $filtername;?></Filter>
        </ClInclude>
<?php }?>
    </ItemGroup>
<?php }?>

<?php if (isset($files['source'])) { ?>
    <ItemGroup>
<?php foreach ($files['source'] as $file) { ?>
        <ClCompile Include="<?php echo $file;?>">
            <Filter><?php echo $filtername;?></Filter>
        </ClCompile>
<?php }?>
    </ItemGroup>
<?php }?>
<?php }?>

</Project>