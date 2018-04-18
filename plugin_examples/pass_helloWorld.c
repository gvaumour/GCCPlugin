#include <gcc-plugin.h>
#include <context.h>

const pass_data pass_data_greg =
{  GIMPLE_PASS, /* type */
  "Greg's Plugin", /* name */
  OPTGROUP_LOOP, /* optinfo_flags */
  true, /* has_gate */
  true, /* has_execute */
  TV_TREE_PREFETCH, /* tv_id */
  ( PROP_cfg | PROP_ssa ), /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

int greg_pass()
{
	printf("### Within Function %s \n", current_function_name());	
	printf("Doing very complex analysis ... \n");	
	return 0;
}

class pass_greg : public gimple_opt_pass
{
public:
  pass_greg (gcc::context ctxt)
    : gimple_opt_pass (pass_data_greg, gcc::context ctxt)
  {}

  bool gate () { return true;}
  unsigned int execute () { return greg_pass (); }

int plugin_init(struct plugin_name_args   *info,  /* Argument infor */
                struct plugin_gcc_version *ver)   /* Version of GCC */
{
//... Some version check and so ...
pass.pass = make_pass_greg(g);
    pass.reference_pass_name = "ssa";
    pass.ref_pass_instance_number = 1;
    pass.pos_op = PASS_POS_INSERT_AFTER;

register_callback("rodetect", PLUGIN_PASS_MANAGER_SETUP, NULL, &pass);

};
