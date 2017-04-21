<?php
/**
 * Posts index renderer - called by the home layout and archives
 *
 * @var config     $config
 * @var posts_data $posts_data
 * @var template   $template
 * @var module[]   $modules
 * @var module     $this_module
 */

use hng2_base\config;
use hng2_base\module;
use hng2_base\template;
use hng2_modules\posts\posts_data;

$this_module = $modules["posts"];

if( in_array($template->get("page_tag"), array("post_category_index", "home"))
    && $account->level >= config::NEWCOMER_USER_LEVEL
    && $settings->get("modules:posts.disable_quick_post_form") != "true" )
    include "{$this_module->abspath}/contents/quick_post_form.inc";

if( $posts_data->count == 0 ) return;

$is_front_page = $posts_data->pagination["this_page_number"] <= 1 ? "true" : "false";

$this_module->load_extensions("front", "index_renderer_before_internal_wrapper");
?>

<div id="posts_index" class="posts_index clearfix" data-is-front-page="<?= $is_front_page ?>"
     data-page-number="<?= $posts_data->pagination["this_page_number"] ?>">
    
    <?
    $config->globals["modules:posts.working_posts_set"] =& $posts_data->posts;
    $this_module->load_extensions("front", "index_renderer_before_loop_start");
    
    $config->globals["posts_rendered_in_index"] = 0;
    foreach( $posts_data->posts as $post ):
        $author = $post->get_author();
        if( $template->get("page_tag") == "home" && $post->pin_to_home == 1 )
            $pinned = "pinned";
        elseif( $template->get("page_tag") == "post_category_index" && $post->pin_to_main_category_index == 1 )
            $pinned = "pinned";
        else $pinned = "";
        
        $special = "";
        if( $post->visibility == "level_based" || $post->main_category_visibility == "level_based" ) $special = " special";
        
        $this_module->load_extensions("front", "index_renderer_item_before");
        ?>
        
        <article class="post clearfix <?= $pinned ?><?= $special ?>" data-post-id="<?= $post->id_post ?>"
                 data-main-category="<?= $post->main_category_slug ?>" data-main-category-min-level="<?= $post->main_category_min_level ?>"
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
                                  onclick="open_level_switcher(this, '<?= $author->id_account ?>', '<?= $author->level ?>', false)">
                                <span class="fa fa-pencil fa-fw"></span>
                                [<?= $author->level ?>] <?= $config->user_levels_by_level[$author->level] ?>
                            </span>
                            <span class="target" style="display: none;"></span>
                        </span>
                        <? if( $modules["messaging"]->enabled ): ?>
                            <span class="meta_field critical pseudo_link" onclick="send_pm(this, '<?= $author->user_name ?>', '<?= htmlspecialchars($author->display_name) ?>')"
                                  title="<?= replace_escaped_vars( $language->contact->pm->title, '{$website_name}', $settings->get("engine.website_name") ) ?>">
                                <span class="fa fa-inbox"></span>
                                <?= $language->contact->pm->caption ?>
                            </span>
                        <? endif; ?>
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
                    
                    <? foreach( $post->get_filtered_tags_list() as $tag): ?>
                        <a class="meta_field" href="<?= $config->full_root_path ?>/tag/<?= $tag ?>" target="_blank">
                            <span class="fa fa-hashtag"></span>
                            <?= $tag ?>
                        </a>
                    <? endforeach; ?>
                    
                    <? $this_module->load_extensions("front", "index_renderer_item_meta_fields_upper"); ?>
                </div>
                <div class="meta_section">
                    <a class="meta_field dimmed" href="<?= $config->full_root_url ?>/date/<?= date("Y/m/d", strtotime($post->publishing_date)) ?>/">
                        <span class="fa fa-calendar fa-fw"></span>
                        <?= time_today_string($post->publishing_date) . " (" . time_elapsed_string($post->publishing_date) . ")" ?>
                    </a>
                    
                    <? if( $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                        <span class="meta_field critical">
                            <span class="fa fa-eye fa-fw"></span>
                            <?= number_format($post->views); ?>
                        </span>
                    <? endif; ?>
                    
                    <? if($post->expiration_date != "0000-00-00 00:00:00"): ?>
                        <span class="meta_field dimmed">
                            <span class="fa fa-clock-o fa-fw"></span>
                            <?= preg_replace('/\d+s/', "", time_remaining_string($post->expiration_date)) ?>
                        </span>
                    <? endif; ?>
                    
                    <? $this_module->load_extensions("front", "index_renderer_item_meta_fields_lower"); ?>
                    
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
            
            <a href="<?= $post->get_permalink() ?>">
                <h1><?= $post->get_processed_title(false) ?></h1>
                <? if( ! empty($post->featured_image_thumbnail) ): ?>
                    <div class="post_featured_image thumbnail <?= $post->featured_media_type ?>">
                        <img src="<?= $post->featured_image_thumbnail ?>">
                    </div>
                <? endif; ?>
            </a>
            
            <div class="post_contents post_excerpt">
                <?
                # echo $post->get_processed_excerpt();
                
                $excerpt = $post->get_processed_excerpt();
                if( ! empty($excerpt) )
                {
                    echo $excerpt;
                }
                else
                {
                    if( empty($post->featured_image_thumbnail) )
                    {
                        $contents = $post->get_processed_content();
                        $pq = phpQuery::newDocumentHTML($contents);
                        $img = $pq->find('img:first');
                        $img->attr("height", "");
                        $img->attr("width", "100%");
                        echo "<a href=\"{$post->get_permalink()}\">$img</a>";
                    }
                }
                ?>
                
                <? if( $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                    <br><a class="greengo inline-block" 
                       href="<?= $config->full_root_path ?>/posts/?edit_post=<?= $post->id_post ?>&wasuuup=<?= md5(mt_rand(1, 65535)) ?>"
                       ><span class="fa fa-pencil fa-fw"></span><?= $language->edit ?></a>
                    <span class="critical inline-block pseudo_link" 
                       onclick="change_post_status( '<?= $post->id_post ?>', 'trashed', this, function() { $('article[data-post-id=' + <?= $post->id_post ?> + ']').fadeOut('fast') } )"
                       ><span class="fa fa-trash fa-fw"></span><?= $this_module->language->record_nav->actions->trash ?></span>
                    <span class="important pseudo_link inline-block"
                          onclick="change_post_status( '<?= $post->id_post ?>', 'reviewing', this, function() { $('article[data-post-id=' + <?= $post->id_post ?> + ']').fadeOut('fast') } )"
                          ><span class="fa fa-flag fa-fw"></span><?= $this_module->language->record_nav->actions->flag_for_review ?>
                    </span>
                <? endif; ?>
                
                <? $this_module->load_extensions("front", "indexes_after_excerpt"); ?>
                
            </div>
            
        </article>
        
        <?
        $config->globals["posts_rendered_in_index"]++;
        $this_module->load_extensions("front", "index_renderer_item_after");
    endforeach;
    ?>
    
</div>

<div id="posts_pagination">
    
    <? $posts_data->browser->render_pagination_links($template->get("pagination_url_prefix"), $posts_data->pagination); ?>
    
</div>
