<?php
/**
 * Media index renderer
 *
 * @var config         $config
 * @var template       $template
 * @var settings       $settings
 * @var items_data     $items_data
 */

use hng2_base\config;
use hng2_base\settings;
use hng2_base\template;
use hng2_modules\gallery\items_data;

$this_module = $modules["gallery"];
?>

<div id="posts_index" class="posts_index clearfix">
    
    <? foreach( $items_data->items as $item):
        $author = $item->get_author();
        $special = "";
        if( $item->visibility == "level_based" || $item->main_category_visibility == "level_based" ) $special = " special";
        ?>
        
        <article class="post <?= $special ?> clearfix" data-post-id="<?= $item->id_post ?>"
                 data-main-category="<?= $item->main_category_slug ?>"
                 data-author-role="<?= $author->get_role(true) ?>" data-author-level="<?= $author->level ?>">
            
            <div class="meta_box clearfix">
                
                <img class="user_avatar" src="<?= $author->get_avatar_url() ?>">
                
                <div class="meta_section upper">
                    <a class="meta_field user_display_name" data-user-level="<?= $author->level ?>"
                       href="<?= $config->full_root_url ?>/user/<?= $author->user_name ?>/">
                        <span class="fa fa-user fa-fw"></span>
                        <?= $author->get_processed_display_name() ?>
                    </a>
                    
                    <? if($account->level >= config::COADMIN_USER_LEVEL && $account->id_account != $author->id_account): ?>
                        <span class="meta_field critical pseudo_link" onclick="if(confirm($_GENERIC_CONFIRMATION)) toggle_account( '<?= $author->id_account ?>', 'disable', this )">
                            <span class="fa fa-lock"></span>
                            <?= $language->disable ?>
                        </span>
                        <span class="user_level_switcher">
                            <span class="current meta_field critical pseudo_link"
                                  onclick="open_level_switcher(this, '<?= $author->id_account ?>', '<?= $author->level ?>', false)">
                                <span class="fa fa-pencil fa-fw"></span>
                                [<?= $author->level ?>] <?= $config->user_levels_by_level[$author->level] ?>
                            </span>
                            <span class="target" style="display: none;"></span>
                        </span>
                        
                        <?= $author->get_pm_sending_link($language->contact->pm->caption, "meta_field critical pseudo_link") ?>
                    <? endif; ?>
                    
                    <a class="meta_field" href="<?= $config->full_root_url ?>/category/<?= $item->main_category_slug ?>/">
                        <span class="fa fa-folder fa-fw"></span>
                        <?= $item->main_category_title ?>
                    </a>
                    <? if( $item->main_category_visibility == "level_based" && ! empty($item->main_category_min_level) ): ?>
                        <span class="meta_field important">
                            <span class="fa fa-users"></span>
                            <?= $config->user_levels_by_level[$item->main_category_min_level]; ?>
                        </span>
                    <? elseif( $item->visibility == "level_based" ): ?>
                        <span class="meta_field important">
                            <span class="fa fa-users"></span>
                            <?= $config->user_levels_by_level[$item->author_level]; ?>
                        </span>
                    <? endif; ?>
                    
                    <? foreach( $item->get_filtered_tags_list() as $tag): ?>
                        <a class="meta_field" href="<?= $config->full_root_path ?>/tag/<?= $tag ?>/media" target="_blank">
                            <span class="fa fa-hashtag"></span>
                            <?= $tag ?>
                        </a>
                    <? endforeach; ?>
                </div>
                <div class="meta_section">
                    <a class="meta_field dimmed" href="<?= $config->full_root_url ?>/date/<?= date("Y/m/d", strtotime($item->publishing_date)) ?>/">
                        <span class="fa fa-calendar fa-fw"></span>
                        <?= time_today_string($item->publishing_date) . " (" . time_elapsed_string($item->publishing_date) . ")" ?>
                    </a>
                    
                    <? if( $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                        <span class="meta_field critical">
                            <span class="fa fa-eye fa-fw"></span>
                            <?= number_format($item->views); ?>
                        </span>
                    <? endif; ?>
                </div>
            
            </div>
            
            <a href="<?= $item->get_page_url() ?>">
                <h1><?= $item->get_processed_title(false) ?></h1>
                <div class="post_featured_image thumbnail <?= $item->type ?>">
                    <? if( $item->type == "image"): ?>
                        <img src="<?= $item->get_thumbnail_url() ?>">
                    <? else: ?>
                        <img class="windowed_image" src="<?= $config->full_root_path ?>/media_toolbox/render_blank_thumbnail"
                             style="background-image: url('<?= $item->get_thumbnail_url() ?>')">
                    <? endif; ?>
                </div>
            </a>
            
            <div class="post_contents post_excerpt">
                <?= $item->get_description_excerpt() ?>
                <? if( $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                    <a class="greengo inline-block" href="<?= $config->full_root_path ?>/gallery/?edit_item=<?= $item->id_media ?>&wasuuup=<?= md5(mt_rand(1, 65535)) ?>"
                       ><span class="fa fa-pencil fa-fw"></span><?= $language->edit ?></a>
                <? endif; ?>
                <? if( $item->can_be_deleted() ): ?>
                    <span class="critical pseudo_link inline-block"
                          onclick="if( confirm($_GENERIC_CONFIRMATION) ) trash_media('<?= $item->id_media ?>', this, function() { $('article[data-post-id=<?= $item->id_post ?>]').fadeOut('fast', function() { $(this).remove(); }) })"
                          ><span class="fa fa-trash fa-fw"></span><?= $current_module->language->record_nav->actions->trash ?></span>
                <? endif; ?>
            </div>
            
        </article>
    
    <? endforeach; ?>
    
</div>

<div id="posts_pagination">
    
    <? $items_data->browser->render_pagination_links($template->get("pagination_url_prefix"), $items_data->pagination); ?>

</div>
