#include "../include/ei_event.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ei_widget_t* actif = NULL;
static ei_default_handle_func_t defaultfun = NULL;


/**
 * Sets the widget which is currently being manipulated by the user.
 *
 * @param	widget		The widget to declare as active, or NULL to declare
 *				that a widget is no more being manipulated.
 */
void ei_event_set_active_widget(ei_widget_t* widget)
{
  actif = widget;
}


/**
 * Returns the widget currently being manipulated by the user.
 *
 * @return			The widget currenlty being manipulated, or NULL.
 */
ei_widget_t* ei_event_get_active_widget()
{
  return actif;
}

 /**
 * Sets the function that must be called when an event has been received but no processed
 *	by any widget.
 *
 * @param	func		The event handling function.
 */
void ei_event_set_default_handle_func(ei_default_handle_func_t func)
{
  defaultfun = func;
}


/**
 * Returns the function that must be called when an event has been received but no processed
 *	by any widget.
 *
 * @return			The address of the event handling function.
 */
ei_default_handle_func_t ei_event_get_default_handle_func()
{
  return defaultfun;
}
