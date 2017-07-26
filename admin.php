<?
/**
 * Base template - Admin
 *
 * @package    HNG2
 * @subpackage core
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var template $template
 * @var settings $settings
 * @var config   $config
 * @var account  $account
 */

use hng2_base\account;
use hng2_base\config;
use hng2_base\settings;
use hng2_base\template;
use hng2_tools\internals;

include __DIR__ . "/functions.inc";
$template->init(__FILE__);
$account->ping();

foreach($template->get_includes("pre_rendering") as $module => $include)
{
    $this_module = $modules[$module];
    include "{$this_module->abspath}/contents/{$include}";
}

header("Content-Type: text/html; charset=utf-8"); ?>
<!DOCTYPE html>
<html>
<head>
    <!--suppress CssInvalidPropertyValue -->
    <style type="text/css">@-ms-viewport{ width: device-width; }</style>
    <meta name="viewport"              content="width=device-width, initial-scale=1">
    <? $template->set("include_notification_functions", true); ?>
    <? $template->set("jquery_ui_theme", "smoothness"); ?>
    <? include ROOTPATH . "/includes/common_header.inc" ?>
    
    <!-- This template -->
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/styles~v1.0.1-2.css">
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/post_styles~v1.0.3-1.css">
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/colors~v1.0.1-5.css">
    
    <? if( $settings->get("engine.hide_left_sidebar_on_admin_pages") != "true" && $template->count_left_sidebar_groups() > 0 ): ?>
        <!-- Left sidebar -->
        <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/left_sidebar_addon~v<?=$config->scripts_version?>.css">
        <script type="text/javascript"          src="<?= $template->url ?>/media/left_sidebar_addon~v<?=$template->version?>-3.js"></script>
    <? endif; ?>
    
    <!-- Always-on -->
    <? $template->render_always_on_files(); ?>
    
    <!-- Per module loads -->
    <?
    foreach($template->get_includes("html_head") as $module => $include)
    {
        $this_module = $modules[$module];
        include "{$this_module->abspath}/contents/{$include}";
    }
    ?>
</head>
<body data-orientation="landscape" data-viewport-class="0" <?=$template->get("additional_body_attributes")?> 
      data-page-tag="<?= $template->get("page_tag") ?>" class="admin"
      data-is-mobile="<?= is_mobile() ? "true" : "false" ?>"
      data-is-known-user="<?= $account->_exists ? "true" : "false" ?>"
      data-user-slug="<?= $account->user_name ?>"
      data-user-level="<?= $account->level ?>">

<div id="body_wrapper">
    
    <?
    foreach($template->get_includes("pre_header") as $module => $include)
    {
        $this_module = $modules[$module];
        include "{$this_module->abspath}/contents/{$include}";
    }
    ?>
    
    <div id="header">
        
        <div class="header_top">
            <?
            if( $settings->get("engine.show_admin_menu_in_header_menu") != "true" )
                include "{$template->abspath}/segments/admin_menu.inc";
            
            foreach($template->get_includes("header_top") as $module => $include)
            {
                $this_module = $modules[$module];
                include "{$this_module->abspath}/contents/{$include}";
            }
            ?>
        </div>
        
        <div class="menu clearfix">
            
            <span id="main_menu_trigger" class="main_menu_item" onclick="toggle_main_menu_items()">
                <span class="fa fa-bars fa-fw"></span>
            </span>
            
            <? if( $settings->get("engine.hide_left_sidebar_on_admin_pages") != "true" && $template->count_left_sidebar_groups() > 0 ): ?>
                <span id="left_sidebar_trigger" class="main_menu_item pull-left"
                      onclick="toggle_left_sidebar_items()">
                    <span class="fa fa-ellipsis-v fa-fw"></span>
                </span>
            <? endif; ?>
            
            <a id="home_menu_button" class="main_menu_item always_visible pull-left" href="<?= $config->full_root_path ?>/">
                <span class="fa fa-home fa-fw"></span>
            </a>
            
            <?
            if( $settings->get("engine.show_admin_menu_in_header_menu") == "true" )
                add_admin_menu_items_to_header_menu();
            
            foreach($template->get_includes("header_menu") as $module => $include)
            {
                $this_module = $modules[$module];
                include "{$this_module->abspath}/contents/{$include}";
            }
            
            echo $template->build_menu_items("priority");
            ?>
        </div>
        
        <div class="header_bottom">
            <?
            foreach($template->get_includes("header_bottom") as $module => $include)
            {
                $this_module = $modules[$module];
                include "{$this_module->abspath}/contents/{$include}";
            }
            ?>
        </div>
        
    </div><!-- /#header -->
    
    <?
    foreach($template->get_includes("pre_content") as $module => $include)
    {
        $this_module = $modules[$module];
        include "{$this_module->abspath}/contents/{$include}";
    }
    ?>
    
    <div id="content_wrapper" class="clearfix">
        
        <? if( $settings->get("engine.hide_left_sidebar_on_admin_pages") != "true" && $template->count_left_sidebar_groups() > 0 ): ?>
            <div id="left_sidebar" class="sidebar">
                <? echo $template->build_left_sidebar_groups(); ?>
            </div>
        <? endif; ?>
        
        <div id="content">
            <? include "{$current_module->abspath}/{$template->page_contents_include}"; ?>
        </div><!-- /#content -->
        
    </div>
    
    <?
    foreach($template->get_includes("post_footer") as $module => $include)
    {
        $this_module = $modules[$module];
        include "{$this_module->abspath}/contents/{$include}";
    }
    ?>
    
    <? if($config->display_performance_details && EMBED_INTERNALS): ?>
        <div id="footer">
            <div class="footer_contents" align="center">
                <?
                if( $config->query_tracking_enabled ) echo "
                    <span class='fa fa-database fa-fw'></span>" . number_format($database->get_tracked_queries_count()) . " •
                    ";
                echo "
                    <span class='fa fa-clock-o fa-fw'></span>" . number_format(microtime(true) - $global_start_time, 3) . "s •
                    <span class='fa fa-tachometer fa-fw'></span>" . number_format(memory_get_usage(true) / 1024 / 1024, 1) . "MiB •
                    <span class='fa fa-plus fa-fw pseudo_link' onclick=\"$('.internals').show();\"></span>
                ";
                ?>
            </div>
        </div><!-- /#footer -->
    <? endif; ?>
    
</div><!-- /#body_wrapper -->

<!-- These must be at the end of the document -->
<script type="text/javascript" src="<?= $config->full_root_path ?>/lib/tinymce-4.6.3/tinymce.min.js"></script>
<? $template->render_tinymce_additions(); ?>
<script type="text/javascript" src="<?= $config->full_root_path ?>/media/init_tinymce~v<?=$config->engine_version?>.js"></script>
<script type="text/javascript">
    $(document).ready(function()
    {
        tinymce.init(tinymce_defaults);
        tinymce.init(tinymce_full_defaults);
        tinymce.init(tinymce_minimal_defaults);
    });
</script>

<?
foreach($template->get_includes("pre_eof") as $module => $include)
{
    $this_module = $modules[$module];
    include "{$this_module->abspath}/contents/{$include}";
}

internals::render(__FILE__);
?>

</body>
</html>
