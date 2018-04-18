#include <gcc-plugin.h>
#include <coretypes.h>
#include <gimple.h>
#include <tree.h>
#include <tree-flow.h>
#include <tree-pass.h>
#include <cfgloop.h>
#include <c-family/c-pragma.h>
#include <cp/cp-tree.h> 
#include "tm.h"
#include <gimple-pretty-print.h>

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "intl.h"


#include <c-family/c-common.h>
#include <c-family/c-pragma.h>

#include <diagnostic.h>

#include <iostream>
#include <set>
#include <vector>


#include "plugin.h"


void loopDetect(){
	
	basic_block bb = ENTRY_BLOCK_PTR;
	loop_p current_loop;
	
	unsigned int nbLoops = number_of_loops();
	for(int i = 0 ; i < nbLoops ; i++){
		current_loop = get_loop(i);
	}
	
	printf("Nombre de loop : %d\n", nbLoops);

}
