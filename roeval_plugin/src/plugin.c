#include <gcc-plugin.h>
#include <coretypes.h>
#include <gimple.h>
#include <tree.h>
#include <tree-flow.h>
#include <tree-pass.h>
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


#include "plugin.h"


using namespace std; 



int plugin_is_GPL_compatible = 1;


/* Help info about the plugin if one were to use gcc's --version --help */
static struct plugin_info rodetect_info =
{
    .version = "0.1",
    .help = "In progress !",
};


static struct plugin_gcc_version rodetect_ver =
{
    .basever = "4.9",
};

/* We don't need to run any tests before we execute our plugin pass */
static bool rodetect_gate(void)
{
    return true;
}

struct local_variable* RW_var[], RO_var[];

static unsigned int rodetect_exec(void)
{

	RW_var.clear();
	RO_var.clear();
	
	printf("### Etude de la fonction %s \n", current_function_name());	
	int action = DETECT_LOOPS;
	
	if(action == DETECT_LOCAL_VAR){
	
	
		detectLocalVar();
		detectAccessLocalVar();
	
		int strategie = AGRESSIVE;
	
		//Strategie d'aobrd conservative ou la variable doit etre en RO sur tous les chemins de controle
		conservativeStrategie();
			 
		if (strategie == AGRESSIVE){
			//Strategie ou la variable doit etre RO sur au moins un chemin de controle
			agressiveStrategie();
		}
	
		printResults();

		RW_var.clear();
		RO_var.clear();
	
	}
	else{
		loopDetect();
	}
	
	return 0;
}

int plugin_init(struct plugin_name_args   *info,  /* Argument infor */
                struct plugin_gcc_version *ver)   /* Version of GCC */
{
	struct register_pass_info pass;

	if (strncmp(ver->basever, rodetect_ver.basever, strlen("4.8")))
		return -1; /* Incorrect version of gcc */
		

	struct opt_pass* pass_rodetect = new opt_pass();
	pass_rodetect->type = GIMPLE_PASS;
	pass_rodetect->name = "rodetect";
	pass_rodetect->gate = rodetect_gate;
	pass_rodetect->execute = rodetect_exec;


	pass.pass = pass_rodetect;
	pass.reference_pass_name = "ssa";
	pass.ref_pass_instance_number = 1;
	pass.pos_op = PASS_POS_INSERT_AFTER;

	/* Tell gcc we want to be called after the first SSA pass */
	
        register_callback("rodetect", PLUGIN_PASS_MANAGER_SETUP, NULL, &pass);
        //register_callback("rodetect", PLUGIN_INFO, NULL, &speller_info);


    return 0;
}
