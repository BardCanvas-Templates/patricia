<?php
/**
 * Comments listing for single post on indexes.
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
 */

use hng2_base\config;
use hng2_base\module;
use hng2_base\settings;
use hng2_base\template;
use hng2_modules\comments\comment_record;
use hng2_modules\comments\comments_repository;
use hng2_modules\posts\post_record;

$excerpt_length = $settings->get("modules:comments.excerpt_lengt_for_indexes");
if( empty($excerpt_length) ) $excerpt_length = 100;

$comments_repository = new comments_repository();
$authors = $comments_repository->get_all_authors($comments);
?>

<ul class="post_comments">
    <? foreach($comments as $comment): ?>
        <li>
            <?
            $config->globals["comments:rendering_comment"] = $comment;
            $this_module->load_extensions("front", "single_comment_before_author_name");
            if( ! empty($comment->id_author) ):
                $author = $authors[$comment->id_author];
                if( ! empty($author) ): ?>
                <a href="<?= $config->full_root_url ?>/user/<?= $author->user_name ?>/" class="user_display_name"
                   data-user-level="<?= $author->level ?>">
                    <span class="fa fa-user fa-fw"></span>
                    <?= $author->get_processed_display_name() ?></a>
                <? else: ?>
                    <span class="fa fa-question-circle-o fa-fw"></span>
                    <?= $language->unknown ?>:
                <? endif; ?>
            <? else: ?>
                <span class="fa fa-question-circle-o fa-fw"></span>
                <?= $comment->author_display_name ?>:
                <?
                $config->globals["comments:rendering_comment"] = $comment;
                $this_module->load_extensions("front", "single_comment_after_anon_name");
                ?>
            <? endif; ?>
            [<?= time_elapsed_string($comment->creation_date) ?>]:
            <a href="<?= $post->get_permalink() ?>#comment_<?= $comment->id_comment ?>">
                <?
                $excerpt = make_excerpt_of($comment->content, $excerpt_length);
                if( empty($excerpt) ) echo "<span class='ellipsis'>...</span>";
                else                  echo convert_emojis($excerpt);
                ?></a>
        </li>
    <? endforeach; ?>
</ul>
