#ifndef PLUGIN_H
#define PLUGIN_H

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

using namespace std; 

struct local_variable
{
	tree var_decl;
	int nbRead;
	int nbWrite;
};

extern struct local_variable* RW_var[];
extern struct local_variable* RO_var[];

#define CONSERVATIVE 0
#define AGRESSIVE 1


#define DETECT_LOCAL_VAR 0
#define DETECT_LOOPS 1


/* Prototypes for the roDetect.c file */ 
static unsigned int rodetect_exec(void);
static bool rodetect_gate(void);
void detectLocalVar();
void printResults();
void detectAccessLocalVar();
void conservativeStrategie();
void agressiveStrategie();
void parcoursCFG(struct local_variable*  watched_RWvar[] , basic_block bb , basic_block *visitedBB );


/* Prototypes for the loopDetect file */
void loopDetect();

#endif 
