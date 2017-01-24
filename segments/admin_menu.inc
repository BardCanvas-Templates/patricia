<?php
/**
 * Admin menu
 *
 * @package    HNG2
 * @subpackage core
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

$menu = array();
$index = 0;
foreach($modules as $module_name => $this_module)
{
    if( ! $account->has_admin_rights_to_module($module_name) ) continue;
    
    if(is_array($this_module->menu_items) || is_object($this_module->menu_items))
    {
        foreach($this_module->menu_items->item as $this_menu_item)
        {
            $strict_item_level = trim($this_menu_item["strict_admin_level"]);
            if( ! empty($strict_item_level) &&  $account->level < $strict_item_level ) continue;
            
            $item_url    = trim($this_menu_item->url);
            $added_class = ($module_name == $current_module->name && stristr($_SERVER["PHP_SELF"], "/$module_name/$item_url") !== false) ? "current" : "";
            $url         = "{$config->full_root_path}/$module_name/$item_url";
            $caption     = $this_module->language->admin->menu_items->{$this_menu_item->caption_language_var};
            $icon        = empty($this_module->language->icon) ? "" : ("<span class='fa " . trim($this_module->language->icon) . "'></span>");
            $key         = strtolower($caption).sprintf("%05.0f", $index);
            $menu[$key]  = "<a class='item $added_class' href='$url'>$icon $caption</a>";
        }
    }
}

if( empty($menu) ) return;

ksort($menu);
?>

<script type="text/javascript">
    function toggle_admin_menu_items()
    {
        var $menu = $('#admin_menu');
        
        if( navigator.userAgent.indexOf('MSIE') > 0 || navigator.userAgent.indexOf('Safari') > 0 )
            $menu.find('.item').toggleClass('visible');
        else
            $menu.find('.item').toggleClass('visible');
        
        $('#admin_menu_trigger').toggleClass('open');
    }
    
    $(document).ready(function()
    {
        $(window).resize(function()
        {
            if( $('#admin_menu_trigger').hasClass('open') )
                toggle_admin_menu_items();
        });
    });
</script>

<div id="admin_menu">
    
    <span id="admin_menu_trigger" onclick="toggle_admin_menu_items()">
        <span class="fa fa-bars"></span>
    </span>
    
    <span class="admin_bar_title <? if($current_module) echo "current"; ?>">
        <span class="fa fa-info-circle"></span>
        <? if($current_module): ?>
            <?= ucwords(str_replace("_", " ", $current_module->name)) ?> v<?= $current_module->version ?>
        <? else: ?>
            <?= $language->admin->bar_title ?> <?= $config->engine_version ?>
        <? endif; ?>
        •
        <span class="fa fa-server"></span>
        #<?= $config->numeric_server_id ?> (<?= current(explode(".", gethostname())) ?>)
    </span>
    
    <?= implode("", $menu); ?>
    
</div>
