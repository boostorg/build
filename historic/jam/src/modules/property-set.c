
#include "../native.h"
#include "../timestamp.h"
#include "../newstr.h"
#include "../strings.h"
#include "../lists.h"
#include "../variable.h"
#include "../compile.h"

/*
rule create ( raw-properties * )
{
    raw-properties = [ sequence.unique 
        [ sequence.insertion-sort $(raw-properties) ] ] ;
         
    local key = $(raw-properties:J=-:E=) ;
    
    if ! $(.ps.$(key)) 
    {
        .ps.$(key) = [ new property-set $(raw-properties) ] ;
    }
    return $(.ps.$(key)) ;    
}
*/

LIST *property_set_create( PARSE *parse, FRAME *frame )
{
    LIST* properties = lol_get( frame->args, 0 );    
    LIST* sorted = list_short(properties);
    LIST* unique = list_unique(sorted);

    LIST* val;
    LIST* tmp;

    string var[1];
    string_new(var);
    string_append(var, ".ps.");
    
    for(tmp = unique; tmp; tmp = tmp->next) {
        string_append(var, tmp->string);
        string_push_back(var, '-');
    }
    val = var_get(var->value);
    if (val == 0) 
    {          
        FRAME       inner[1];
        frame_init( inner );
        inner->prev = frame;
        inner->module = frame->module;
        inner->procedure = 0;
        
        lol_add( inner->args, list_append(list_new(0, "property-set"), unique));
        
        val = evaluate_rule("new", inner);
        
        frame_free(inner);
        
        var_set(newstr(var->value), list_copy(0, val), VAR_SET);
    }
    else
    {
        val = list_copy(0, val);
    }
    
    string_free(var);
    list_free(sorted);
    list_free(unique);

    return val;

}

void init_property_set()
{
    {
        char* args[] = { "raw-properties", "*", 0 };
        declare_native_rule("property-set", "create", args, property_set_create);
    }

}
