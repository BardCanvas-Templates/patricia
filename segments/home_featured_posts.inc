<?php
/**
 * Featured posts
 *
 * @var template   $template
 * @var posts_data $posts_data
 * 
 * @var module $this_module
 */

use hng2_base\config;
use hng2_base\module;
use hng2_base\template;
use hng2_modules\posts\posts_data;

$this_module = $modules["posts"];
?>

<div id="featured_posts_column">
    <?
    $config->globals["modules:posts.working_posts_set"] =& $posts_data->featured_posts;
    $this_module->load_extensions("front", "featured_posts_before_loop_start");
    $ttl = $settings->get("modules:posts.featured_posts_ttl");
    
    foreach($posts_data->featured_posts as $post):
        $src = $post->featured_image_thumbnail;
        if( empty($post->featured_image_thumbnail) ) $src = $settings->get("modules:posts.default_featured_image");
        $pinned = $post->pin_to_home == 1 ? "pinned" : "";
        $special = $post->visibility == "level_based" || $post->main_category_visibility == "level_based" ? " special" : ""; ?>
        
        <a class="featured_post <?= $pinned ?><?= $special ?>" href="<?= $post->get_permalink() ?>" style="position: relative;">
            <? if($account->level >= config::MODERATOR_USER_LEVEL ): ?>
                <span class="pseudo_link fa fa-times fa-border"
                      style="color: red; border-color: red; background-color: rgba(0, 0, 0, 0.5); position: absolute; z-index: 1; right: 5px; top: 5px;"
                      onclick="remove_tag_from_post($(this).closest('.featured_post'), '<?= $post->id_post ?>', '<?= $settings->get("modules:posts.featured_posts_tag") ?>'); return false;"></span>
            <? endif; ?>
            <? if( ! empty($src) ): ?><span 
                class="thumbnail <?= $post->featured_media_type ?>"><img src="<?= $src ?>"></span><? endif; ?>
            <h3>
                <?= $post->get_processed_title(false) ?>
                [<?= $post->get_processed_author_display_name() ?>, <?= $post->main_category_title ?>]
                <? if($post->expiration_date != "0000-00-00 00:00:00"): ?>
                    <span style="display: inline-block">
                        <span class="fa fa-clock-o fa-fw"></span>
                        <?= preg_replace('/\d+s/', "", time_remaining_string($post->expiration_date)) ?>
                    </span>
                <? endif; ?>
            </h3>
            <? if($account->level >= config::MODERATOR_USER_LEVEL ): ?>
                <div class="admin_stuff">
                    <span class="critical inline-block"><span
                          class="fa fa-calendar fa-fw"></span><?= time_elapsed_string($post->publishing_date) ?></span>
                    <? if( ! empty($ttl) ): ?>
                        <span class="critical inline-block"><span 
                              class="fa fa-clock-o fa-fw"></span><?= 
                            preg_replace('/\d+s/', "", time_remaining_string(date("Y-m-d H:i:s", strtotime("{$post->publishing_date} + $ttl hours")))) 
                            ?></span>
                    <? endif; ?>
                    <? foreach($post->get_filtered_tags_list() as $tag): ?>
                        <span class="critical inline-block"><span class="fa fa-hashtag"></span><?= $tag ?></span>
                    <? endforeach; ?>
                </div>
            <? endif; ?>
        </a>
    <? endforeach; ?>
</div>
