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


void agressiveStrategie(){
	
	
	gimple stmt;
	vector<local_variable*> watched_RWvar;
	
	for(int i = 0 ; i < RW_var.size() ; i++){
		struct local_variable* current_var = new local_variable();
		current_var->var_decl  = RW_var[i]->var_decl;
		current_var->nbRead  = 0;
		current_var->nbWrite  = 0;
		watched_RWvar.push_back(current_var);	
	}
	
	basic_block bb = ENTRY_BLOCK_PTR;
	parcoursCFG(watched_RWvar, bb , list<basic_block>(0) );

}



void fusion_RO_var(vector<struct local_variable*> RO_vars_possib){

	for(int i = 0 ; i < RO_vars_possib.size() ; i++){
		bool find = false;
	
		for(int j = 0 ; j < RO_var.size() ; j++ ){
			if(RO_var[j]->var_decl == RO_vars_possib[i]->var_decl){
				find = true;	
				break;
			}
		}
	
		if(!find){
			RO_var.push_back(RO_vars_possib[i]);
			for(int j = 0 ; j < RW_var.size() ; j++ ){
				if(RW_var[j]->var_decl == RO_vars_possib[i]->var_decl){
					RW_var.erase(RW_var.begin() + j);
					break;
				}
			}
		}
	}


}
void parcoursCFG(vector<struct local_variable*>  watched_RWvar , basic_block bb , list<basic_block> visitedBB ){

	gimple stmt;
	gimple_stmt_iterator gsi;


	for (gsi=gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
	{
		stmt = gsi_stmt(gsi);

		struct use_optype_d* use_operands = gimple_use_ops (stmt);
		int i=0;
		const_tree op;
		
		 for (int i=0; i<gimple_num_ops(stmt); ++i){
 		 	if ((op = gimple_op(stmt, i)) ){
 		 		
 		 		if(TREE_CODE(op) == SSA_NAME){
 		 			tree ssa_name = SSA_NAME_VAR(op);

 		 			if(!ssa_name)
 		 				continue;
 		 				
	 				for(int j = 0; j < watched_RWvar.size() ; j++){	
	 										
	 					if(watched_RWvar.at(j)->var_decl == ssa_name){	
 		 					if(i == 0)
 		 						watched_RWvar.at(j)->nbWrite++;
 		 					else
 		 						watched_RWvar.at(j)->nbRead++;
	 						break;
	 					}
 		 			}
 		 		}
 		 	}			 
		 }	
	}


	if(EXIT_BLOCK_PTR == bb){
		//Last BB of the function	
		vector<local_variable*> detectedRO_var(0);
		
		printf("Nom\tRead\tWrite\n");
		for(int i = 0 ; i < watched_RWvar.size() ; i++){
			
			printf("%s\t%d\t%d\n" , get_name(watched_RWvar.at(i)->var_decl), watched_RWvar.at(i)->nbRead, watched_RWvar.at(i)->nbWrite);
		 	
			if(watched_RWvar.at(i)->nbWrite <= 1 && watched_RWvar.at(i)->nbRead >= 1){
				detectedRO_var.push_back(watched_RWvar.at(i));
			}
			
		}
		fusion_RO_var(detectedRO_var);
	}
	else{
		edge e;
		edge_iterator ei;


		
		FOR_EACH_EDGE (e, ei, bb->succs)
		{
			
			//printf(" BB n°%d has successors %d\n" , bb->index , e->dest->index);
			bool alreadyVisited = false ;
			for (list<basic_block>::iterator it=visitedBB.begin(); it != visitedBB.end(); ++it){
				if(*it == e->dest)
					alreadyVisited = true;
			}
			if(!alreadyVisited){
				list<basic_block> new_list(visitedBB);
				new_list.push_back(bb);
				
				vector<local_variable*> new_RWvars(0);
				for(int i = 0 ; i < watched_RWvar.size() ; i++){
					struct local_variable* new_RWvar = new local_variable();
					new_RWvar->var_decl = watched_RWvar.at(i)->var_decl;
					new_RWvar->nbRead = watched_RWvar.at(i)->nbRead;
					new_RWvar->nbWrite = watched_RWvar.at(i)->nbWrite;
					
					new_RWvars.push_back(new_RWvar);						 
				}	
				
					
				parcoursCFG(new_RWvars , e->dest, new_list ) ;
	

			}
			else{
				cout << "\tArret du chemin" << endl;	
			}
			
		}
	
	}


}


void conservativeStrategie(){

	//cout << "Nom\tRead\tWrite" << endl;
	
	vector<int> tabIndex(0);
	
	for(int i = 0 ; i< RW_var.size() ; i++){
		if(RW_var.at(i)->nbWrite <= 1 &&  RW_var.at(i)->nbRead >= 1){
			tabIndex.push_back(i);
			printf("Variable %s detected as RO \n", get_name(RW_var.at(i)->var_decl) );
		}
	}
	
	for(int i = tabIndex.size()-1 ; i >= 0 ; i--){
		local_variable* ROvar = RW_var.at(tabIndex[i]);
		RW_var.erase(RW_var.begin() + tabIndex[i]);
		RO_var.push_back(ROvar);
	}

}

void detectAccessLocalVar(){

	basic_block bb;
	gimple stmt;
		
	FOR_EACH_BB(bb){	

		gimple_stmt_iterator gsi;
		for (gsi=gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
		{
			stmt = gsi_stmt(gsi);

			struct use_optype_d* use_operands = gimple_use_ops (stmt);
			int i=0;
			const_tree op;
			
			 for (int i=0; i<gimple_num_ops(stmt); ++i){
         		 	if ((op = gimple_op(stmt, i)) ){
         		 		
         		 		if(TREE_CODE(op) == SSA_NAME){
         		 			tree ssa_name = SSA_NAME_VAR(op);

         		 			if(!ssa_name)
         		 				continue;
         		 				
 		 				for(int j = 0; j < RW_var.size() ; j++){	
 		 										
 		 					if(RW_var.at(j)->var_decl == ssa_name){	
	 		 					if(i == 0)
	 		 						RW_var.at(j)->nbWrite++;
	 		 					else
	 		 						RW_var.at(j)->nbRead++;
 		 						break;
 		 					}
	 		 			}
         		 		}
         		 	}			 
			 }	
		}
	}
	

}

void detectLocalVar(){
	
	int u;
	tree list;

	FOR_EACH_LOCAL_DECL (cfun, u, list) 
	{ 
	
		if(list){
		
			struct local_variable* new_var = new local_variable();
			new_var->var_decl = list;
			
			const char* name = get_name (list);
			if(name){
			
				if(TYPE_READONLY(list)){
					//printf("Variable %s est en read-only \n", name);
					new_var->nbRead = -1;
					new_var->nbWrite = -1;					
					RO_var.push_back(new_var);
					
				}
				else{
					new_var->nbRead = 0;
					new_var->nbWrite = 0;
					RW_var.push_back(new_var);
					//printf("Variable : %s\n",  name);
				}	
			}				 
		}
	} 
}

void printResults(){
	
	printf("RW Variable: ");
	for(int i = 0 ; i < RW_var.size() ; i++){
		printf("%s", get_name(RW_var[i]->var_decl) );
		if( i != RW_var.size() -1)
			printf(",");
	}
	printf("\n");

	
	printf("Variable en RO : ");
	for(int i = 0 ; i < RO_var.size() ; i++){
		printf("%s", get_name(RO_var[i]->var_decl) );
		if( i != RO_var.size() -1)
			printf(",");
	}
	printf("\n");
}


