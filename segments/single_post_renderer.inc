<?php
/**
 * Single post renderer
 *
 * @var config      $config
 * @var module      $current_module posts
 * @var post_record $post
 */

use hng2_base\config;
use hng2_base\module;
use hng2_modules\posts\post_record;

$author = $post->get_author();
?>

<article class="post single_post <?= $post->status ?> clearfix" data-post-id="<?= $post->id_post ?>"
         data-main-category="<?= $post->main_category_slug ?>" data-author-id="<?= $author->id_account ?>"
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
                <span class="meta_field critical pseudo_link disable_user_link" onclick="if(confirm($_GENERIC_CONFIRMATION)) toggle_account( '<?= $author->id_account ?>', 'disable', this )">
                    <span class="fa fa-lock"></span>
                    <?= $language->disable ?>
                </span>
                <span class="user_level_switcher">
                    <span class="current meta_field critical pseudo_link"
                          onclick="open_level_switcher(this, '<?= $author->id_account ?>', '<?= $author->level ?>')">
                        <span class="fa fa-pencil fa-fw"></span>
                        [<?= $author->level ?>] <?= $config->user_levels_by_level[$author->level] ?>
                    </span>
                    <span class="target" style="display: none;"></span>
                </span>
            <? endif; ?>
            
            <a class="meta_field" href="<?= $config->full_root_url ?>/category/<?= $post->main_category_slug ?>/">
                <span class="fa fa-folder fa-fw"></span>
                <?= $post->main_category_title ?>
            </a>
            <? if( $post->main_category_visibility == "level_based" && ! empty($post->main_category_min_level) ): ?>
                <span class="meta_field important">
                    <span class="fa fa-users"></span>
                    <?= $config->user_levels_by_level[$post->main_category_min_level]; ?>
                </span>
            <? elseif( $post->visibility == "level_based" ): ?>
                <span class="meta_field important">
                    <span class="fa fa-users"></span>
                    <?= $config->user_levels_by_level[$post->author_level]; ?>
                </span>
            <? endif; ?>
            
            <? if( $account->level < config::MODERATOR_USER_LEVEL ): ?>
                <? foreach( $post->get_filtered_tags_list() as $tag): ?>
                    <a class="meta_field" href="<?= $config->full_root_path ?>/tag/<?= $tag ?>" target="_blank">
                        <span class="fa fa-hashtag"></span>
                        <?= $tag ?>
                    </a>
                <? endforeach; ?>
            <? else: ?>
                <? if( is_array($post->tags_list) ) foreach( $post->tags_list as $tag): ?>
                    <a class="meta_field" href="<?= $config->full_root_path ?>/tag/<?= $tag ?>" target="_blank">
                        <span class="fa fa-hashtag"></span>
                        <?= $tag ?>
                    </a>
                <? endforeach; ?>
            <? endif; ?>
        </div>
        <div class="meta_section">
            <a class="meta_field dimmed" href="<?= $config->full_root_url ?>/date/<?= date("Y/m/d", strtotime($post->publishing_date)) ?>/">
                <span class="fa fa-calendar fa-fw"></span>
                <?= time_complete_string($post->publishing_date) . " (" . time_elapsed_string($post->publishing_date) . ")" ?>
            </a>
            <span class="meta_field">
                <span class="fa fa-eye fa-fw"></span>
                <?= number_format($post->views); ?>
            </span>
            <? if($post->expiration_date != "0000-00-00 00:00:00"): ?>
                <span class="meta_field important">
                    <span class="fa fa-clock-o fa-fw"></span>
                    <?= preg_replace('/\d+s/', "", time_remaining_string($post->expiration_date)) ?>
                </span>
            <? endif; ?>
            
            <? if( ! empty($post->creation_ip) && $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                <span class="meta_field critical">
                            <span class="fa fa-cloud"></span>
                    <?= $post->creation_ip ?>
                        </span>
                <a class="meta_field critical" href="<?= $config->full_root_path ?>/?s=ip:<?= $post->creation_ip ?>">
                    <span class="fa fa-search"></span>
                    <?= $language->ip ?>
                </a>
                <? $parts = explode(".", $post->creation_ip); array_pop($parts); ?>
                <a class="meta_field critical" href="<?= $config->full_root_path ?>/?s=ip:<?= implode(".", $parts) ?>.%25">
                    <span class="fa fa-search"></span>
                    <?= $language->segment ?>
                </a>
    
                <?
                $parts   = explode("; ", $post->creation_location);
                $isp      = array_pop($parts);
                $country  = array_pop($parts);
                $city     = implode("; ", $parts);
                if( $city != "N/A; N/A") : ?>
                    <a class="meta_field critical" href="<?= $config->full_root_path ?>/?s=city:<?= urlencode($city) ?>">
                        <span class="fa fa-map-marker"></span>
                        <?= $city ?>
                    </a>
                <? endif; ?>
                <? if($country != "N/A"): ?>
                    <a class="meta_field critical" href="<?= $config->full_root_path ?>/?s=country:<?= urlencode($country) ?>">
                        <span class="fa fa-globe"></span>
                        <?= $country ?>
                    </a>
                <? endif; ?>
                <? if($isp != "N/A"): ?>
                    <a class="meta_field critical" href="<?= $config->full_root_path ?>/?s=isp:<?= urlencode($isp) ?>">
                        <span class="fa fa-building"></span>
                        <?= $isp ?>
                    </a>
                <? endif; ?>
            <? endif; ?>
        </div>
        
    </div>
    
    <h1>
        <?= $post->get_processed_title() ?>
    </h1>
    
    <? if($post->status == "reviewing"): ?>
        <div class="framed_content state_ko">
            <span class="fa fa-warning"></span>
            <?= $current_module->language->messages->post_being_reviewed ?>
        </div>
    <? endif; ?>
    
    <p class="single_item_actions">
        <span class="item action clipboard-copy" data-clipboard-text="<?= $post->get_permalink(true) ?>" style="">
            <?= $language->permalink ?>
            <span class="fa fa-clipboard"></span>
        </span>
        
        <? if( $post->can_be_edited() ): ?>
            <a class="item action" href="<?= $config->full_root_path ?>/posts/?edit_post=<?= $post->id_post ?>&wasuuup=<?= md5(mt_rand(1, 65535)) ?>">
                <span class="fa fa-pencil"></span>
                <?= $language->edit ?>
            </a>
        <? endif; ?>
        
        <? if( $post->can_be_deleted() ): ?>
            <span class="item action" onclick="change_post_status( '<?= $post->id_post ?>', 'trashed', this, function() { location.href = $_FULL_ROOT_PATH + '/' } )">
                <span class="fa fa-trash"></span>
                <?= $current_module->language->record_nav->actions->trash ?>
            </span>
        <? endif; ?>
        
        <?= $author->get_pm_sending_link($language->pm_author, "item action") ?>
        
        <? if( $account->_exists || $settings->get("modules:contact.no_anonymous_emails") != "true" ): ?>
            <? if( $author->get_engine_pref("@contact:allow_emails") != "false"
                   || $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                <a class="item action" href="<?= $config->full_root_path ?>/contact/?target=<?= urlencode($author->user_name) ?>">
                    <span class="fa fa-envelope"></span>
                    <?= $language->email_author ?>
                </a>
            <? endif; ?>
        <? endif; ?>
        
        <? if( $account->level < config::MODERATOR_USER_LEVEL && $account->id_account != $author->id_account ): ?>
            <a class="item action" href="<?= $config->full_root_path ?>/contact/?action=report&type=post&id=<?= $post->id_post ?>">
                <span class="fa fa-exclamation-circle"></span>
                <?= $language->report_to_webmaster ?>
            </a>
        <? endif; ?>
        
        <? if( $account->level >= config::MODERATOR_USER_LEVEL && $post->status == "published" ): ?>
            <span class="item admin action"
                  onclick="change_post_status( '<?= $post->id_post ?>', 'reviewing', this, function() { location.reload(); } )">
                <span class="fa fa-flag"></span>
                <?= $current_module->language->record_nav->actions->flag_for_review ?>
            </span>
        <? endif; ?>
        
        <? if( $account->level >= config::MODERATOR_USER_LEVEL && $post->status == "reviewing" ): ?>
            <span class="item admin action"
                  onclick="change_post_status( '<?= $post->id_post ?>', 'published', this, function() { location.reload(); } )">
                <span class="fa fa-check"></span>
                <?= $current_module->language->record_nav->actions->publish ?>
            </span>
        <? endif; ?>
        
        <? $current_module->load_extensions("front", "single_post_actions"); ?>
        
    </p>
    
    <div class="post_contents">
        <?= $post->get_processed_content() ?>
        <? $current_module->load_extensions("front", "single_post_content_additions"); ?>
    </div>
    
    <? if( ! empty($author->signature) ): ?>
        <div class="author_signature">
            <?= $author->get_processed_signature() ?>
        </div>
    <? endif ?>
    
</article>

<? $current_module->load_extensions("front", "single_post_after_contents"); ?>
