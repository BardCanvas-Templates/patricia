<?php
/**
 * Comments listing for single post.
 * Note: this is included directly, so no globals are imported.
 *
 * @package    BardCanvas
 * @subpackage comments
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var config   $config
 * @var settings $settings
 * @var template $template
 * @var module   $this_module    self (comments)
 * @var module   $current_module posts
 *             
 * Trailing vars:
 * @var post_record      $post
 * @var comment_record[] $comments
 * @var boolean          $allow_new_comments
 */

use hng2_base\config;
use hng2_base\module;
use hng2_base\settings;
use hng2_base\template;
use hng2_modules\comments\comment_record;
use hng2_modules\comments\comments_repository;
use hng2_modules\posts\post_record;
use hng2_tools\record_browser;

$browser    = new record_browser("");
$repository = new comments_repository();

list($find_params, $comments_count, $comments, $pagination) = $repository->get_for_single_post($post->id_post);
if( $comments_count == 0 && $post->allow_comments == 0 ) return;

/** @var comment_record[] $parents */
$parents = array();
foreach($comments as $comment) $parents[$comment->id_comment] = $comment;

$config->globals["comments:current_comments_set"] =& $comments;
$this_module->load_extensions("front", "comments_for_single_post_presets");
?>

<div id="post_comments" class="content_frame" data-post-id="<?= $post->id_post ?>">
    
    <h1>
        <? if( $post->allow_comments && $allow_new_comments ): ?>
            <!--suppress HtmlUnknownAnchorTarget -->
            <a href="#post_new_comment_form" class="pull-right"><?= $this_module->language->list->post_comment ?></a>
        <? endif; ?>
        
        <?= replace_escaped_vars($this_module->language->list->title, '{$amount}', $comments_count) ?>
    </h1>
    
    <? foreach($comments as $comment):
        $author     = $comment->get_author();
        $avatar_url = $author->get_avatar_url();
        $max_level  = $settings->get("modules:comments.max_indent_level"); if( empty($max_level) ) $max_level = 5;
        $level      = $comment->indent_level <= $max_level ? $comment->indent_level : $max_level;
        
        $config->globals["comments:rendering_comment"] = $comment; ?>
        
        <div class="comment_entry" id="comment_<?= $comment->id_comment ?>" data-id-comment="<?= $comment->id_comment ?>"
             data-parent-comment="<?= $comment->parent_comment ?>" data-indent-level="<?= $level ?>"
             data-post-id="<?= $post->id_post ?>">
            
            <div class="comment_wrapper">
                
                <? $this_module->load_extensions("front", "single_comment_before_meta_box"); ?>
                
                <div class="meta_box clearfix">
                    <img class="user_avatar" src="<?= $avatar_url ?>">
                    
                    <div class="meta_section upper">
                        <? if($author->_exists): ?>
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
                            <? endif; ?>
                        <? else: ?>
                            <span class="meta_field">
                                <span class="fa fa-question-circle-o fa-fw"></span>
                                <?= $comment->author_display_name ?>
                                <? $this_module->load_extensions("front", "single_comment_after_anon_name"); ?>
                            </span>
                        <? endif; ?>
                        
                        <?
                        if( ! empty($comment->parent_comment) )
                        {
                            $parent = $parents[$comment->parent_comment];
                            if( ! empty($parent) )
                            {
                                if( empty($parent->id_author) )
                                {
                                    echo "
                                        <span class='fa fa-chevron-right fa-fw'></span>
                                        <span class='fa fa-question-circle'></span>
                                        {$parent->author_display_name}
                                    ";
                                }
                                else
                                {
                                    $parent_author = $parent->get_author();
                                    echo "
                                        <span class='fa fa-chevron-right fa-fw'></span>
                                        <a class='user_display_name inline-block' data-user-level='{$parent_author->level}'
                                           href='{$config->full_root_path}/user/{$parent_author->user_name}'>
                                            <span class='fa fa-user fa-fw'></span>
                                            {$parent_author->get_processed_display_name()}
                                        </a>
                                    ";
                                }
                            }
                        }
                        ?>
                    </div>
                    <div class="meta_section">
                        <span class="meta_field dimmed">
                            <span class="fa fa-calendar fa-fw"></span>
                            <?= time_today_string($comment->creation_date) . " (" . time_elapsed_string($comment->creation_date) . ")" ?>
                        </span>
                        
                        <? if( ! empty($comment->creation_ip) && $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                            <span class="meta_field critical">
                                <span class="fa fa-cloud"></span>
                                <?= $comment->creation_ip ?>
                            </span>
                            <a class="meta_field critical" href="<?= $config->full_root_path ?>/comments/?search_for=<?= $comment->creation_ip ?>">
                                <span class="fa fa-search"></span>
                                <?= $language->ip ?>
                            </a>
                            <? $parts = explode(".", $comment->creation_ip); array_pop($parts); ?>
                            <a class="meta_field critical" href="<?= $config->full_root_path ?>/comments/?search_for=<?= implode(".", $parts) ?>.">
                                <span class="fa fa-search"></span>
                                <?= $language->segment ?>
                            </a>
                            
                            <?
                            $parts   = explode("; ", $comment->creation_location);
                            $isp      = array_pop($parts);
                            $country  = array_pop($parts);
                            $city     = implode("; ", $parts);
                            if( $city != "N/A; N/A") : ?>
                                <a class="meta_field critical" href="<?= $config->full_root_path ?>/comments/?search_for=<?= urlencode($city) ?>">
                                    <span class="fa fa-map-marker"></span>
                                    <?= $city ?>
                                </a>
                            <? endif; ?>
                            <? if($country != "N/A"): ?>
                                <a class="meta_field critical" href="<?= $config->full_root_path ?>/comments/?search_for=<?= urlencode($country) ?>">
                                    <span class="fa fa-globe"></span>
                                    <?= $country ?>
                                </a>
                            <? endif; ?>
                            <? if($isp != "N/A"): ?>
                                <a class="meta_field critical" href="<?= $config->full_root_path ?>/comments/?search_for=<?= urlencode($isp) ?>">
                                    <span class="fa fa-building"></span>
                                    <?= $isp ?>
                                </a>
                            <? endif; ?>
                        <? endif; ?>
                    </div>
                </div>
                
                <? $this_module->load_extensions("front", "single_comment_after_meta_box"); ?>
                
                <div class="content">
                    <? $this_module->load_extensions("front", "single_comment_before_content"); ?>
                    <?= $comment->get_processed_content() ?>
                    <? $this_module->load_extensions("front", "single_comment_after_content"); ?>
                </div>
                
                <? $this_module->load_extensions("front", "single_comment_before_wrapper_closing"); ?>
                
            </div>
            
            <? $this_module->load_extensions("front", "single_comment_before_reply_actions"); ?>
            
            <div class="comment_reply">
                <div class="trigger">
                    
                    <? $this_module->load_extensions("front", "single_comment_pre_actions1"); ?>
                    
                    <a class="comment_action_permalink" href="<?= $comment->get_permalink() ?>" target="_blank">
                        <span class="fa fa-link"></span>
                        <?= $language->permalink ?>
                    </a>
                    
                    <? $this_module->load_extensions("front", "single_comment_pre_actions2"); ?>
                    
                    <? if($post->allow_comments && $allow_new_comments): ?>
                        <span class="comment_action_reply pseudo_link reply_comment_trigger" onclick="prepare_comment_reply(this, false, true)">
                            <span class="fa fa-reply"></span>
                            <?= $this_module->language->list->reply_comment ?>
                        </span>
                        <span class="comment_action_quote pseudo_link quote_comment_trigger" onclick="prepare_comment_reply(this, true, true)">
                            <span class="fa fa-quote-right"></span>
                            <?= $this_module->language->list->quote_comment ?>
                        </span>
                    <? endif; ?>
                    
                    <? if( $account->_exists && $account->id_account != $comment->id_author ): ?>
                        <span class="comment_action_flag_as_spam pseudo_link"
                              onclick="if( confirm($_GENERIC_CONFIRMATION) ) spam_comment('<?= $comment->id_comment ?>', function() { $('.comment_entry[data-id-comment=<?= $comment->id_comment ?>]').fadeOut('fast', function() { $(this).remove(); }); }, this)">
                            <span class="fa fa-flag"></span>
                            <?= $language->flag_as_spam ?>
                        </span>
                        <? if($account->level < config::MODERATOR_USER_LEVEL): ?>
                            <a class="comment_action_report_to_webmaster" href="<?= "{$config->full_root_url}/contact/?action=report&type=comment&id={$comment->id_comment}" ?>" target="_blank">
                                <span class="fa fa-exclamation-circle"></span>
                                <?= $language->report_to_webmaster ?>
                            </a>
                        <? endif; ?>
                    <? endif; ?>
                    
                    <? if( $account->level >= config::MODERATOR_USER_LEVEL
                           && $comment->id_author != $account->id_account
                           && in_array($comment->status, array("published", "reviewing", "spam")) ): ?>
                        <span class="comment_action_reject pseudo_link important"
                              onclick="if( confirm($_GENERIC_CONFIRMATION) ) reject_comment('<?= $comment->id_comment ?>', function() { $('.comment_entry[data-id-comment=<?= $comment->id_comment ?>]').fadeOut('fast', function() { $(this).remove(); }); })">
                            <span class="fa fa-times"></span>
                            <?= $this_module->language->record_nav->actions->reject ?>
                        </span>
                    <? endif; ?>
                    
                    <? if( ! is_null($author) && $account->id_account != $author->id_account ): ?>
                        
                        <?= $author->get_pm_sending_link($language->pm_author, "comment_action_pm_author pseudo_link") ?>
                        
                        <? if( $account->_exists || $settings->get("modules:contact.no_anonymous_emails") != "true" ): ?>
                            <? if( $author->get_engine_pref("@contact:allow_emails") != "false"
                                   || $account->level >= config::MODERATOR_USER_LEVEL ): ?>
                                <a class="comment_action_email_author" href="<?= $config->full_root_path ?>/contact/?target=<?= urlencode($author->user_name) ?>">
                                    <span class="fa fa-envelope"></span>
                                    <?= $language->email_author ?>
                                </a>
                            <? endif; ?>
                        <? endif; ?>
                    <? endif; ?>
                    
                    <? if( is_comment_editable($comment) ): ?>
                        <a class="comment_action_edit" href="<?= $config->full_root_path ?>/comments/?edit=<?= $comment->id_comment ?>">
                            <span class="fa fa-pencil"></span>
                            <?= $this_module->language->record_nav->actions->edit ?>
                        </a>
                    <? endif; ?>
                    
                    <? if( $comment->can_be_deleted() ): ?>
                        <span class="pseudo_link comment_action_trash"
                              onclick="if( confirm($_GENERIC_CONFIRMATION) ) delete_comment('<?= $comment->id_comment ?>', function() { $('.comment_entry[data-id-comment=<?= $comment->id_comment ?>]').fadeOut('fast', function() { $(this).remove(); }); }, this)">
                            <span class="fa fa-trash"></span>
                            <?= $this_module->language->record_nav->actions->trash ?>
                        </span>
                    <? endif; ?>
                    
                    <? $this_module->load_extensions("front", "single_comment_actions"); ?>
                    
                </div>
                <div class="target"></div>
            </div>
            
            <? $this_module->load_extensions("front", "single_comment_after_reply_actions"); ?>
            
        </div>
        
    <? endforeach; ?>
    
    <?
    if( $comments_count > $find_params->limit )
    {
        echo "<p id='posts_pagination'>";
        $browser->render_pagination_links($template->get("pagination_url_prefix"), $pagination);
        echo "</p>";
    }
    ?>
    
</div>

<div id="comments_edit_form" style="display: none;" data-title-template="<?= htmlspecialchars($this_module->language->form->edit) ?>"></div>
<div id="comments_edit_target" style="display: none;"></div>
<div id="reply_target" style="display: none;"></div>

<? if( $_POST["prevent_reopen"] != "true" ): ?>
    <script type="text/javascript">
        $(document).ready(function()
        {
            var comment_id = '<?= $_GET["reply_to"] ?>';
        
            if( comment_id == '' ) return;
        
            var $comment = $('.comment_entry[data-id-comment="' + comment_id + '"]');
            if( $comment.length == 0 ) return;
        
            var $trigger = $comment.find('.comment_reply .reply_comment_trigger');
            setTimeout(function() { prepare_comment_reply($trigger, false, true) }, 1000);
        });
    </script>
<? endif; ?>

<? if( $_GET["offset"] != "" ): ?>
    <script type="text/javascript">
        $(document).ready(function()
        {
            $.scrollTo( $('#post_comments') );
        });
    </script>
<? endif; ?>

<? $this_module->load_extensions("front", "comments_for_single_post_before_eof"); ?>
