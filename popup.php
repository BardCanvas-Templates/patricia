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
    <? include __DIR__ . "/segments/common_header.inc"; ?>
    
    <!-- Others -->
    <script type="text/javascript" src="<?= $config->full_root_path ?>/lib/jquery.blockUI.js"></script>
    <script type="text/javascript" src="<?= $config->full_root_path ?>/lib/jquery.form.min.js"></script>
    
    <!-- Core functions and styles -->
    <link rel="stylesheet" type="text/css" href="<?= $config->full_root_path ?>/media/styles~v<?=$config->scripts_version?>.css">
    <? if($account->_is_admin): ?><link rel="stylesheet" type="text/css" href="<?= $config->full_root_path ?>/media/admin~v<?=$config->scripts_version?>.css"><? endif; ?>
    <script type="text/javascript"          src="<?= $config->full_root_path ?>/media/functions~v<?=$config->scripts_version?>.js"></script>
    
    <!-- This template -->
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/styles~v<?=$config->scripts_version?>.css">
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/post_styles~v<?=$config->scripts_version?>.css">
    <link rel="stylesheet" type="text/css" href="<?= $template->url ?>/media/colors~v<?=$config->scripts_version?>.css">
    
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
