<?
/**
 * Base template - Embedded layout
 *
 * @package    HNG2
 * @subpackage core
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_tools\internals;

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
    <? $template->set("include_notification_functions", false); ?>
    <? $template->set("jquery_ui_theme", "smoothness"); ?>
    <? include ROOTPATH . "/includes/common_header.inc" ?>
    
    <!-- This template -->
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/styles~v1.0.1-2.css">
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/post_styles~v1.0.3-1.css">
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/colors~v1.0.1-5.css">
    
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
<body data-orientation="landscape" data-viewport-class="0" <?=$template->get("additional_body_attributes")?>  class="popup"
      data-is-known-user="<?= $account->_exists ? "true" : "false" ?>"
      data-user-level="<?= $account->level ?>">

<div id="body_wrapper">
    
    <div id="content">
        <? include "{$current_module->abspath}/contents/{$template->page_contents_include}"; ?>
    </div><!-- /#content -->
    
    <?
    foreach($template->get_includes("post_footer") as $module => $include)
    {
        $this_module = $modules[$module];
        include "{$this_module->abspath}/contents/{$include}";
    }
    ?>
    
</div><!-- /#body_wrapper -->

<? internals::render(__FILE__); ?>

</body>
</html>
