<?php
/**
 * Search by tag results
 *
 * @var config     $config
 * @var template   $template
 * @var module[]   $modules
 * @var string     $tag
 */

use hng2_base\config;
use hng2_base\module;
use hng2_base\template;
use hng2_modules\search\search_by_tag_item;

/* @var search_by_tag_item[][] $collection */
$collection =& $config->globals["search_by_tag_collections"];

$this_module   = $modules["search"];
$columns_count = count($config->globals["search_by_tag_collections"]);
$column_size   = empty($columns_count) ? 100 : floor(100 / $columns_count);
?>
<style type="text/css">
    
    @media all and (min-width: 1024px) {
        #hashtag_entries_index .column {
            display: inline-block;
            width: <?= "calc({$column_size}% - 10px)" ?>;
            margin-right: 10px;
            vertical-align: top;
        }
        
        #hashtag_entries_index .column:last-child {
            margin-right: 0;
        }
        
        #column_selector_tabs { display: none; }
    }
    
    #hashtag_entries_index article {
        margin-bottom: 20px; background-color: white;
        -webkit-box-shadow: 1px 1px 5px 0 rgba(0, 0, 0, 0.25);
        -moz-box-shadow:    1px 1px 5px 0 rgba(0, 0, 0, 0.25);
        box-shadow:         1px 1px 5px 0 rgba(0, 0, 0, 0.25);
        padding: 10px;
    }
    
    #hashtag_entries_index article h3 {
        margin-top: 0; margin-bottom: 5px;
    }
    
    #hashtag_entries_index article .hashtag_entry_thumbnail img {
        width: 100%;
    }
    
    #hashtag_entries_index article .hashtag_entry_contents {
        margin-top: 5px;
    }
    
    #column_selector_tabs .tab {
        display: inline-block; padding: 5px 10px; margin: 2px 0;
    }
</style>

<div id="column_selector_tabs">
    <? foreach($collection as $segment_title => $search_items): ?>
        <a class="framed_content tab" href="#<?=wp_sanitize_filename($segment_title)?>"
           onmouseover="$(this).toggleClass('state_hover', true)"
           onmouseout="$(this).toggleClass('state_hover', false)">
            <?= $segment_title ?> (<?= count($search_items) ?>)
        </a>
    <? endforeach; ?>
</div>

<div id="hashtag_entries_index" class="clearfix">
    
    <? foreach($collection as $segment_title => $search_items): ?>
        
        <a name="<?=wp_sanitize_filename($segment_title)?>"></a>
        <div class="column">
            
            <h2><?= $segment_title ?></h2>
            
            <? foreach($search_items as $item): ?>
                
                <article class="hashtag_entry clearfix">
                    <a href="<?= $item->url ?>">
                        <h3><?= $item->title ?></h3>
                        <? if( ! empty($item->thumbnail) ): ?>
                            <div class="hashtag_entry_thumbnail <?= $item->type ?>">
                                <img class="lazyload"
                                     src="<?= $config->full_root_path ?>/media_toolbox/render_blank_thumbnail"
                                     data-original="<?= $item->thumbnail ?>">
                            </div>
                        <? endif; ?>
                    </a>
                    
                    <? if( ! empty($item->excerpt) ): ?>
                        <div class="hashtag_entry_contents">
                            <?= $item->excerpt ?>
                        </div>
                    <? endif; ?>
                </article>
                
            <? endforeach; ?>
            
        </div>
        
    <? endforeach; ?>
    
</div>
