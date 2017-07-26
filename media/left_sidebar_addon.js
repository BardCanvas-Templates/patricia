
var window_width_for_left_sidebar = 0;

function toggle_left_sidebar_items()
{
    window_width_for_left_sidebar = $(window).width();
    
    var $sidebar       = $('#left_sidebar');
    var visible        = $sidebar.is(':visible');
    var bar_visibility = visible ? 'false' : 'true';
    
    $('body').attr('data-left-sidebar-visible', bar_visibility);
    
    if( visible )
    {
        $sidebar.css('margin-top', '0');
        
        return;
    }
    
    var $menu = $('#header').find('.menu');
    var top   = 0 - $('#content_wrapper').offset().top + $menu.offset().top + $menu.height();
    $sidebar.css('margin-top', top + 'px');
}

$(document).ready(function()
{
    if( $(window).width() < 480 )
        $('body').attr('data-left-sidebar-visible', 'false');
    
    $(window).resize(function()
    {
        if( $('body').attr('data-left-sidebar-visible') != 'true' ) return;
            
        var width = $(window).width();
        if( width == window_width_for_left_sidebar ) return;
        
        toggle_left_sidebar_items();
    });
});
