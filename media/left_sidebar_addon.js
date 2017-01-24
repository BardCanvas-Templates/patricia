
var window_width_for_left_sidebar = 0;

function toggle_left_sidebar_items()
{
    window_width_for_left_sidebar = $(window).width();
    
    var visible        = $('#left_sidebar').is(':visible');
    var bar_visibility = visible ? 'false' : 'true';
    
    $('body').attr('data-left-sidebar-visible', bar_visibility);
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
