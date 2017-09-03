/********************************************//**
 * \file   event_scheduling.cpp
 * \brief  Events list implementation.
 * \author Krzysztof Bakowski (krzysztof.bakowski@put.poznan.pl)
 * \date   2013-05-08
 ***********************************************/
#include "sc.h"
/////////////////////////////////////////////////
template <class object> void Rel<object>::follow(Rel* item)
{
	this->prev = item;			// The item becomes as previous element on the list.
	this->next = item->next;	// The next element assignment.
	item->next = this->next->prev = this;
}
//----------------------------------------------
template <class object> void Rel<object>::remove()
{
	prev->next=this->next;
	next->prev=this->prev;
	delete this;
}
//===============================================
EventList::EventList()
{
	head = new Rel<Event>(new nil(-1.));
}
//----------------------------------------------
Event* EventList::first()
{
	return head->next->data;
}
//----------------------------------------------
Event* EventList::remove_first()
{
	Event* current = first();      // Pointer to the first event on the list.
	head->next->remove();		   // Remove the indicated element of the link class.
	return current;
}
//----------------------------------------------
void EventList::schedule(Event * to_occur)
{
	Rel<Event>* n = NULL;

	// Searching from the end.
	for(n = head->prev; n->data->event_time > to_occur->event_time; n = n->prev);
	
	(new Rel<Event>(to_occur))->follow(n);
}
//----------------------------------------------
bool EventList::empty()
{
	return head->next == head;
}
//----------------------------------------------
void EventList::make_empty()
{
	while(!empty()) delete this->remove_first();
}
/////////////////////////////////////////////////
//***********************************************
