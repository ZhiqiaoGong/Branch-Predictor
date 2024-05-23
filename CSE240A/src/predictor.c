
// //========================================================//
// //  predictor.c                                           //
// //  Source file for the Branch Predictor                  //
// //                                                        //
// //  Implement the various branch predictors below as      //
// //  described in the README                               //
// //========================================================//
// #include <stdio.h>
// #include "predictor.h"

// //
// // TODO:Student Information
// //
// const char *studentName = "Brandon Saldanha, Mihir Pratap Singh";
// const char *studentID   = "A59011109, A59011168";
// const char *email       = "bsaldanha@ucsd.edu, m8singh@ucsd.edu";

// //------------------------------------//
// //      Predictor Configuration       //
// //------------------------------------//

// // Handy Global for use in output routines
// const char *bpName[4] = { "Static", "Gshare",
//                           "Tournament", "Perceptron" };

// int ghistoryBits; // Number of bits used for Global History
// int lhistoryBits; // Number of bits used for Local History
// int pcIndexBits;  // Number of bits used for PC index
// int bpType;       // Branch Prediction Type
// int verbose;

// //------------------------------------//
// //      Predictor Data Structures     //
// //------------------------------------//

// //? Add your own Branch Predictor data structures here

// //Gshare
// uint8_t *bht_gshare;
// uint64_t ghistory;

// //Tournamnet
// uint8_t *gpt;
// uint8_t *cpt;
// uint8_t *lpt;
// uint8_t *lht;
// uint64_t phistory;

// //Perceptron
// int **perceptrons;
// int perceptron_history_length;
// int n_perceptrons;
// int threshold;

// //------------------------------------//
// //        Predictor Functions         //
// //------------------------------------//


// //Fuction declarations:
// void init_gshare();
// void train_gshare(uint32_t pc, uint8_t outcome);
// uint8_t make_prediction_gshare(uint32_t pc);

// void init_tourn();
// void train_tourn(uint32_t pc, uint8_t outcome);
// uint8_t make_prediction_tourn(uint32_t pc);
// uint8_t findChoice(uint32_t pc);

// // Initialize the predictor
// //
// void init_predictor()
// {
// 	//? Initialize Branch Predictor Data Structures
// 	switch (bpType){
// 		case STATIC:
// 			break;
// 		case GSHARE:
// 			init_gshare();
// 			break;
// 		case TOURNAMENT:
// 			init_tourn();
// 			break;

// 		default:
// 			break;
// 	}

// }

// // Make a prediction for conditional branch instruction at PC 'pc'
// // Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// // indicates a prediction of not taken
// //
// uint8_t make_prediction(uint32_t pc)
// {
//   	//
//   	//TODO: Implement prediction scheme

//   	switch (bpType) {
//     	case STATIC:
//     		return TAKEN;
//     	case GSHARE:
// 			return make_prediction_gshare(pc);
//     	case TOURNAMENT:
// 			return make_prediction_tourn(pc);

//     	default:
//     		return NOTTAKEN;
//   	}

//   	// If there is not a compatable bpType then return NOTTAKEN
//   	return NOTTAKEN;
// }

// // Train the predictor the last executed branch at PC 'pc' and with
// // outcome 'outcome' (true indicates that the branch was taken, false
// // indicates that the branch was not taken)
// //
// void train_predictor(uint32_t pc, uint8_t outcome)
// {
//   	//
//   	//? Implement Predictor training
// 	switch (bpType){
// 		case STATIC:
// 			break;
// 		case GSHARE:
// 			train_gshare(pc, outcome);
// 			break;
// 		case TOURNAMENT:
// 			train_tourn(pc, outcome);
// 			break;
// 		default:
// 			break;
// 	}
// }


// //*Gshare functions:

// void init_gshare(){
//   // Allocate an array to use as BHT
//   // Size of BHT depends on number of bits used 
//   int bht_entries = 1 << ghistoryBits;
//   bht_gshare = (uint8_t*)malloc(bht_entries * sizeof(uint8_t));
//   int i = 0;
//   for(i = 0; i< bht_entries; i++){
//     bht_gshare[i] = WN;
//   }
//   ghistory = 0;
// }

// uint8_t make_prediction_gshare(uint32_t pc){
//   //get lower ghistoryBits of pc
//   	uint32_t mask = ~(UINT32_MAX << ghistoryBits);
// 	uint32_t pcLowerBits = pc & mask;
// 	uint32_t ghistoryLowerBits = ghistory & mask;
// 	uint32_t index = ghistoryLowerBits ^ pcLowerBits;

// 	// uint32_t bht_entries = 1 << ghistoryBits;
//   	// uint32_t pc_lower_bits = pc & (bht_entries-1);
//   	// uint32_t ghistory_lower_bits = ghistory & (bht_entries -1);
//   	// uint32_t index = pc_lower_bits ^ ghistory_lower_bits;


//   // if(verbose){
//   //   printf("bht_entries: %d, pc_lower_bits: %d, ghistory: %lu, index: %d \n", bht_entries, pc_lower_bits, ghistory, index);
//   // }
//   switch(bht_gshare[index]){
//     case WN:
//       return NOTTAKEN;
//     case SN:
//       return NOTTAKEN;
//     case WT:
//       return TAKEN;
//     case ST:
//       return TAKEN;
//     default:
//       printf("Warning: Undefined state of entry in GSHARE BHT!\n");
//       return NOTTAKEN;
//   }
// }

// //GSHARE Training
// void train_gshare(uint32_t pc, uint8_t outcome){
// 	uint32_t mask = ~(UINT32_MAX << ghistoryBits);
// 	uint32_t pcLowerBits = pc & mask;
// 	uint32_t ghistoryLowerBits = ghistory & mask;
// 	uint32_t index = ghistoryLowerBits ^ pcLowerBits;

// 	// uint32_t bht_entries = 1 << ghistoryBits;
//   	// uint32_t pc_lower_bits = pc & (bht_entries-1);
//   	// uint32_t ghistory_lower_bits = ghistory & (bht_entries -1);
//   	// uint32_t index = pc_lower_bits ^ ghistory_lower_bits;

// 	//Update counters
// 	switch (bht_gshare[index])
// 	{
//     case WN:
//       bht_gshare[index] = (outcome==TAKEN)?WT:SN;
//       break;
//     case SN:
//       bht_gshare[index] = (outcome==TAKEN)?WN:SN;
//       break;
//     case WT:
//       bht_gshare[index] = (outcome==TAKEN)?ST:WN;
//       break;
//     case ST:
//       bht_gshare[index] = (outcome==TAKEN)?ST:WT;
//       break;
//     default:
//       printf("Warning: Undefined state of entry in GSHARE BHT!\n");
//   }

//   //Update history register
//   ghistory = ((ghistory << 1) | outcome); 
// }

// void cleanup_gshare(){
//   free(bht_gshare);
// }


// // * Tounrmanent functions

// void init_tourn(){

// 	int g_entries = 1 << ghistoryBits;
// 	int lpt_entries = 1 << lhistoryBits;
// 	int lht_entries = 1 << pcIndexBits;
// 	gpt = (uint8_t*)malloc(g_entries * sizeof(uint8_t));
// 	cpt = (uint8_t*)malloc(g_entries * sizeof(uint8_t));
// 	lpt = (uint8_t*)malloc(lpt_entries * sizeof(uint8_t));
// 	lht = (uint8_t*)malloc(lht_entries * sizeof(uint8_t));
// 	phistory=0;

// 	int i = 0;
// 	for(i = 0; i< g_entries; i++){
// 		gpt[i] = WN;
// 		cpt[i] = WT;
// 	}
// 	for(i = 0; i< lpt_entries; i++){
// 		lpt[i] = WN;
// 	}
// 	for(i = 0; i< lht_entries; i++){
// 		lht[i] = 0;
// 	}
// }


// uint8_t findChoice(uint32_t pc){

// 	uint32_t andbits = 1<<pcIndexBits;
// 	uint32_t pc_lower_bits = pc&(andbits-1);
// 	uint32_t l_val = lpt[lht[pc_lower_bits]];
// 	uint32_t g_val = gpt[phistory];

// 	uint32_t g_pred = NOTTAKEN;
// 	uint32_t l_pred = NOTTAKEN;
// 	//printf("%d %d\n",g_val,l_val);
// 	switch(l_val){
// 	case WN:
// 		l_pred= NOTTAKEN;
// 		break;
// 	case SN:
// 		 l_pred= NOTTAKEN;
// 		 break;
// 	case WT:
// 		l_pred= TAKEN;
// 		break;
// 	case ST:
// 		 l_pred=  TAKEN;
// 		 break;
// 	default:
// 		printf("Warning: Undefined state of entry in l_val!\n");
// 	}
		
// 	switch(g_val){
// 	case WN:
// 		g_pred= NOTTAKEN;
// 		break;
// 	case SN:
// 		 g_pred= NOTTAKEN;
// 		 break;
// 	case WT:
// 		g_pred= TAKEN;
// 		break;
// 	case ST:
// 		 g_pred=  TAKEN;
// 		 break;
// 	default:
// 		printf("Warning: Undefined state of entry in g_val\n");
// 	}

// 	uint32_t choice = (g_pred << 1) | l_pred;

// 	return choice;
// }

// uint8_t make_prediction_tourn(uint32_t pc){
// 	uint32_t choice = findChoice(pc);
// 	switch(choice){
// 	case WN:
// 		if(cpt[phistory]>1)
// 			return NOTTAKEN;
// 		else 
// 			return TAKEN;
// 	case SN:
// 		 return NOTTAKEN;
// 	case WT:
// 		if(cpt[phistory]>1)
// 			return TAKEN;
// 		else
// 			return NOTTAKEN;
// 	case ST:
// 		 return TAKEN;
// 	default:
// 		printf("Warning: Undefined state of entry in choice!\n");
// 		return NOTTAKEN;
// 		}
// }

// void train_tourn(uint32_t pc, uint8_t outcome){
// 	uint32_t choice= findChoice(pc);
// 	uint32_t andbits=1<<pcIndexBits;
// 	uint32_t pc_lower_bits=pc&(andbits-1);
// 	int g_entries = 1 << ghistoryBits;
// 	int lht_entries = 1 << pcIndexBits;

// 	if(outcome==TAKEN){
// 		if(lpt[lht[pc_lower_bits]]<3)
// 			lpt[lht[pc_lower_bits]]+=1;

// 		if(gpt[phistory]<3)
// 			gpt[phistory]+=1;
// 	}
    	
// 	if(outcome==NOTTAKEN){
// 		if(lpt[lht[pc_lower_bits]]>0)
// 			lpt[lht[pc_lower_bits]]-=1;

// 		if(gpt[phistory]>0)
// 			gpt[phistory]-=1;
// 	}
    	
// 	lht[pc_lower_bits]=(lht[pc_lower_bits]<<1) | outcome;
// 	lht[pc_lower_bits]= lht[pc_lower_bits] & (lht_entries -1);
    
// 	int pred_choice = cpt[phistory];	
// 	switch(choice){
//     case WN:
//     	if(outcome == 1){
//     		cpt[phistory] = (pred_choice>0)?pred_choice-1:pred_choice;
//     	}
//     	else{
//     	   cpt[phistory] = (pred_choice<3)?pred_choice+1:pred_choice;
//     	}
//       break;
//     case SN:
//       break;
//     case WT:
//     	if(outcome == 1){
//     		cpt[phistory] = (pred_choice<3)?pred_choice+1:pred_choice;
//     	}
//     	else{
//     	  cpt[phistory] = (pred_choice>0)?pred_choice-1:pred_choice;
//     	}
//       break;
//     case ST:
//       break;
//     default:
//       printf("Warning: Undefined state of entry in train!\n");
//   }
// 	phistory = (phistory << 1) | outcome;
// 	phistory = phistory & (g_entries - 1);
// }



// void cleanup_tourn(){
//   free(gpt);
//   free(cpt);
//   free(lpt);
//   free(lht);
// }


// // * Perceptron functions

// void init_perceptron(){
//   	ghistory = 0;
//   	threshold = (1.93*perceptron_history_length) + 14;
//   	int i =0;
//   	int j = 0;
//   	perceptrons = (int**)malloc(n_perceptrons * sizeof(int*));
//   	for(i= 0; i<n_perceptrons;i++){
//     	perceptrons[i] = (int*)malloc((perceptron_history_length+1) * sizeof(int));
//     	for(j = 0; j<perceptron_history_length+1; j++){
//     		perceptrons[i][j] = 1;
//     	}
//   	}
// }

// int compute_perceptron_result(uint32_t pc){
//   	int index = (pc>>2)%n_perceptrons;
//   	int result = perceptrons[index][0];
//   	int i = 0;
//   	uint64_t temp = ghistory;
//   	for(i = 1; i< perceptron_history_length+1; i++){
//   	  	if((temp & (1<<i)) != 0){
//   	  	  	result += perceptrons[index][i];
//   	  	}
//   	  	else{
//   	  	  	result -= perceptrons[index][i];
//   	  	}
//   	}
//   	return result;
// }

// uint8_t make_prediction_perceptron(uint32_t pc){
//   	int result = compute_perceptron_result(pc);
//   	return (result>0)?TAKEN:NOTTAKEN;
// }

// void train_perceptron(uint32_t pc, uint8_t outcome){
//   	int index = (pc>>2)%n_perceptrons;
//   	int result = compute_perceptron_result(pc);
//   	int sign = (outcome == TAKEN)?1:-1;
//   	int i = 0;
//   	uint64_t temp = ghistory;
//   	if((result>0 && outcome == NOTTAKEN) || abs(result) < threshold){
//   	  	for(i = 1; i< perceptron_history_length+1; i++){
//   	  	  	if((temp & (1<<i)) != 0){
//   	  	  	  	perceptrons[index][i] = perceptrons[index][i] + sign;
//   	  	  	}
//   	  	  	else{
//   	  	  	  	perceptrons[index][i] = perceptrons[index][i] - sign;
//   	  	  	}
//   	  	}
//   	  	perceptrons[index][0] = perceptrons[index][0] + sign;
//   	}
//   	ghistory = ((ghistory << 1) | outcome); 
//   	ghistory = ghistory & ((1<<perceptron_history_length)-1);
// }

// void cleanup_perceptron(){
//   	int i = 0;
//   	for(i= 0; i<n_perceptrons;i++){
//   	  	free(perceptrons[i]);
//   	}
//   	free(perceptrons);
// }



// // Free any dynamically allocated Datastructures
// void cleanup() {
//   	switch (bpType) {
//     case STATIC:
//     	break;
//     case GSHARE:
//       	cleanup_gshare();
//       	break;
//     case TOURNAMENT:
//       	cleanup_tourn();
//       	break;
//     default:
//       	break;
//   }
// }


/*

//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"
#include <string.h>

//
// TODO:Student Information
//
const char *studentName = "NAME";
const char *studentID   = "PID";
const char *email       = "EMAIL";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History, for gshare, tournament
int lhistoryBits; // Number of bits used for Local History, for tour P1 simple BHT
int pcIndexBits;  // Number of bits used for PC index, for counter and tour P2 correlation, and custon
int choiceBits;   // Number of bits used for PC index, for custom
int bpType;       // Branch Prediction Type
int verbose;

// gshare global
uint32_t gshareHistory;  // history 10101...
uint8_t *gshareBHT;      // length * state, ST | WT | WN | SN

// tournament global
uint32_t tourHistory;    
uint8_t *globalBHT;        // P1, global
uint8_t *localBHT;         // P2, local
uint8_t *localCounter;     // P2, local
uint8_t *chooser;          // 0| 1| 2| 3, each time plus (P1 correct - P2 correct)
uint8_t globalPred, localPred;  // gloabal and local prediction results

// custom global, bi - mode
uint32_t bimodeHistory;
uint8_t *directionBHT1;
uint8_t *directionBHT2;
uint8_t *choiceTable;
uint8_t pred1, pred2; 
uint8_t choosed;


//------------------------------------//
//       Initialize Functions         //
//------------------------------------//

void init_gshare(){
  gshareHistory = 0;
  uint32_t length = 1 << ghistoryBits;  // 2 ^ ghistoryBits combinations
  gshareBHT = malloc(length * sizeof(uint8_t));
  memset(gshareBHT, SN, length * sizeof(uint8_t));
}

void init_tournament(){
  tourHistory = 0;
  chooser      = malloc((1 << ghistoryBits) * sizeof(uint8_t));
  globalBHT    = malloc((1 << ghistoryBits) * sizeof(uint8_t));
  localBHT     = malloc((1 << pcIndexBits)  * sizeof(uint8_t));
  localCounter = malloc((1 << lhistoryBits) * sizeof(uint8_t));
  memset(chooser,      3 , (1 << ghistoryBits) * sizeof(uint8_t));     // choose global when starts
  memset(globalBHT,    WN, (1 << ghistoryBits) * sizeof(uint8_t));
  memset(localBHT,     0 , (1 << pcIndexBits)  * sizeof(uint8_t));     // lhistory starts with 0
  memset(localCounter, WN, (1 << lhistoryBits) * sizeof(uint8_t));
}

void init_custom(){
  bimodeHistory = 0;
  directionBHT1 = malloc((1 << pcIndexBits) * sizeof(uint8_t));
  directionBHT2 = malloc((1 << pcIndexBits) * sizeof(uint8_t));
  choiceTable   = malloc((1 << choiceBits ) * sizeof(uint8_t));
  memset(directionBHT1, WN , (1 << pcIndexBits) * sizeof(uint8_t));
  memset(directionBHT2, WN , (1 << pcIndexBits) * sizeof(uint8_t));
  memset(choiceTable  , 1  , (1 << choiceBits ) * sizeof(uint8_t));  // 0, 1, 2, 3
  // printf("info custom: %d, %d\n", pcIndexBits, choiceBits);
}

// Initialize the predictor
//
void init_predictor(){
  //
  //TODO: Initialize Branch Predictor Data Structures
  init_gshare();
  init_tournament();
  init_custom();
}


//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken

uint8_t pred_gshare(uint32_t pc){
  uint32_t mask = (1 << ghistoryBits) - 1;
  uint32_t idx = (pc ^ gshareHistory) & mask;         // index to find an entry in BTH
  if (gshareBHT[idx] == ST || gshareBHT[idx] == WT){
    return TAKEN;
  } else{
    return NOTTAKEN;
  }
}

uint8_t pred_tournament(uint32_t pc){
  uint32_t g_idx = tourHistory & ((1 << ghistoryBits) - 1);
  uint8_t choice = chooser[g_idx];

  // global
  if (globalBHT[g_idx] == ST || globalBHT[g_idx] == WT){
    globalPred = TAKEN;
  } else{
    globalPred = NOTTAKEN;
  }

  // local
  uint32_t pc_idx = pc & ((1 << pcIndexBits) - 1);
  uint32_t lhistory = localBHT[pc_idx];
  if (localCounter[lhistory] == ST || localCounter[lhistory] == WT){
    localPred = TAKEN;
  } else{
    localPred = NOTTAKEN;
  }

  // choose
  if (choice == 0 || choice == 1){
    return localPred;
  } else{
    return globalPred;
  }
}

uint8_t pred_custom(uint32_t pc){
  uint32_t dir_idx = (bimodeHistory ^ pc) & ((1 << pcIndexBits) - 1);
  if (directionBHT2[dir_idx] == ST || directionBHT2[dir_idx] == WT){
    pred2 = TAKEN;
  } else{
    pred2 = NOTTAKEN;
  }
  if (directionBHT1[dir_idx] == ST || directionBHT1[dir_idx] == WT){
    pred1 = TAKEN;
  } else{
    pred1 = NOTTAKEN;
  }

  uint32_t choice_idx = pc & ((1 << choiceBits) - 1);
  if (choiceTable[choice_idx] == 0 || choiceTable[choice_idx] == 1){
    // choose directionBHT 1
    choosed = 1;
    return pred1;
  } 
  else{
    // choose directionBHT 2
    choosed = 2;
    return pred2;
  }
  
}

uint8_t make_prediction(uint32_t pc){
  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
      return pred_gshare(pc);
      break;
    case TOURNAMENT:
      return pred_tournament(pc);
      break;
    case CUSTOM:
      return pred_custom(pc);
      break;
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

//------------------------------------//
//           Update Predictor         //
//------------------------------------//
// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)

void train_gshare(uint32_t pc, uint8_t outcome){
  uint32_t mask = (1 << ghistoryBits) - 1;
  uint32_t idx = (pc ^ gshareHistory) & mask;
  if (outcome == TAKEN && gshareBHT[idx] != 3){
    gshareBHT[idx] += 1;
  }
  else if (outcome == NOTTAKEN && gshareBHT[idx] != 0){
    gshareBHT[idx] -= 1;
  }
  gshareHistory <<= 1;
  gshareHistory |= outcome;
}

void train_tournament(uint32_t pc, uint8_t outcome){
  // global
  uint32_t g_idx = tourHistory & ((1 << ghistoryBits) - 1);
  if (outcome == TAKEN && globalBHT[g_idx] != 3){
    globalBHT[g_idx] += 1;
  }
  else if (outcome == NOTTAKEN && globalBHT[g_idx] != 0){
    globalBHT[g_idx] -= 1;
  }

  // local
  uint32_t pc_idx = pc & ((1 << pcIndexBits) - 1);
  uint32_t lhistory = localBHT[pc_idx];
  if (outcome == TAKEN && localCounter[lhistory] != 3){
    localCounter[lhistory] += 1;
  }
  else if (outcome == NOTTAKEN && localCounter[lhistory] != 0){
    localCounter[lhistory] -= 1;
  }
  localBHT[pc_idx] <<= 1;
  localBHT[pc_idx] |= outcome;

  // chooser
  uint8_t globalCorrect = (outcome == globalPred) ? 1 : 0;
  uint8_t localCorrect  = (outcome == localPred) ? 1 : 0;
  chooser[g_idx] += (globalCorrect - localCorrect);
  if (chooser[g_idx] > 3) chooser[g_idx] = 3;
  if (chooser[g_idx] < 0) chooser[g_idx] = 0;

  tourHistory <<= 1;
  tourHistory |= outcome;
}

void train_custom(uint32_t pc, uint8_t outcome){
  // direction predictors
  uint32_t dir_idx = (bimodeHistory ^ pc) & ((1 << pcIndexBits) - 1);
  uint8_t pred, oppositePred;
  if (choosed == 1){
    pred = pred1;
    oppositePred = pred2;
    if (outcome == TAKEN && directionBHT1[dir_idx] != 3){
      directionBHT1[dir_idx] += 1;
    }
    else if (outcome == NOTTAKEN && directionBHT1[dir_idx] != 0){
      directionBHT1[dir_idx] -= 1;
    }
  } else{
    pred = pred2;
    oppositePred = pred1;
    if (outcome == TAKEN && directionBHT2[dir_idx] != 3){
      directionBHT2[dir_idx] += 1;
    }
    else if (outcome == NOTTAKEN && directionBHT2[dir_idx] != 0){
      directionBHT2[dir_idx] -= 1;
    }
  }

  // choice predictor
  uint32_t choice_idx = pc & ((1 << choiceBits) - 1);
  if (pred != outcome && oppositePred == outcome){
    if (choosed == 1){ 
      // 1 wrong but 2 correct
      choiceTable[choice_idx] += 1;
      if (choiceTable[choice_idx] > 3) choiceTable[choice_idx] = 3;
    } else{
      // 2 wrong but 1 correct
      choiceTable[choice_idx] -= 1;
      if (choiceTable[choice_idx] < 0) choiceTable[choice_idx] = 0;
    }
  }


  bimodeHistory <<= 1;
  bimodeHistory |= outcome;
}

void train_predictor(uint32_t pc, uint8_t outcome){
  //
  //TODO: Implement Predictor training
  //
  switch (bpType) {
    case GSHARE:
      train_gshare(pc, outcome);
      break;
    case TOURNAMENT:
      train_tournament(pc, outcome);
      break;
    case CUSTOM:
      train_custom(pc, outcome);
      break;
    default:
      break;
  }
}*/


//
//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include <math.h>
#include "predictor.h"
#include <string.h>
#include <stdbool.h>

//
// TODO:Student Information
//
const char *studentName = "Yilan Chen";
const char *studentID   = "A59002673";
const char *email       = "yic031@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

//define number of bits required for indexing the BHT here. 
int ghistoryBits = 8; // Number of bits used for Global History
int bpType;       // Branch Prediction Type
int verbose;

int pcIndexBits = 8;  // Number of bits used for PC index, determine the size of the pattern history table (PHT)
int lhistoryBits = 8; // Number of bits used for Local History, determine the size of the branch history table (BHT)


int ghistoryBits_custom = 8; // Number of bits used for Global History, determine the size of the Global branch history table (BHT)
int pcIndexBits_custom = 8;  // Number of bits used for PC index, determine the size of the pattern history table (PHT)
int lhistoryBits_custom = 8; // Number of bits used for Local History, determine the size of the branch history table (BHT)


//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//gshare
uint8_t *gshareBHT; // Branch History Table for GShare
uint64_t globalHistory; // Global History Register

//tournament
uint32_t *localPatternHistoryTable; // Local Pattern History Table (PHT)
uint32_t *localBranchHistoryTable; // Local Branch History Table (BHT)
uint32_t *globalBranchHistoryTable; // Global Branch History Table (BHT)
uint32_t *chooserTable; // Chooser Table for selecting between local and global prediction

uint32_t globalHistoryMask; // Mask for global history
uint32_t localHistoryMask; // Mask for local history
uint32_t pcIndexMask; // Mask for PC index


//------------------------------------//
//        Predictor Functions         //
//------------------------------------//


// Initialize the GShare predictor
void initGShare() {
    int numEntries = 1 << ghistoryBits; // Calculate the number of entries
    gshareBHT = (uint8_t*)malloc(numEntries * sizeof(uint8_t));
    for (int i = 0; i < numEntries; i++) {
        gshareBHT[i] = WN; // Initialize to weakly not taken
    }
    globalHistory = 0; // Reset global history
}


// Predict the outcome using GShare
uint8_t gsharePredict(uint32_t pc) {
    uint32_t mask = (1 << ghistoryBits) - 1; // Mask for indexing
    uint32_t index = (pc ^ globalHistory) & mask; // Calculate index
    switch (gshareBHT[index]) {
        case WN: return NOTTAKEN;
        case SN: return NOTTAKEN;
        case WT: return TAKEN;
        case ST: return TAKEN;
        default: 
            printf("Undefined"); 
            return NOTTAKEN;
    }
}

// Train the GShare predictor
void trainGShare(uint32_t pc, uint8_t outcome) {
    uint32_t mask = (1 << ghistoryBits) - 1; // Mask for indexing
    uint32_t index = (pc ^ globalHistory) & mask; // Calculate index

    // Update BHT based on outcome
    if (outcome == TAKEN) {
        if (gshareBHT[index] < ST) gshareBHT[index]++;
    } else {
        if (gshareBHT[index] > SN) gshareBHT[index]--;
    }

    // Update global history
    globalHistory = (globalHistory << 1) | outcome;
}

// Clean up the GShare predictor
void cleanupGShare() {
    free(gshareBHT);
}




// Initialize the tournament predictor
void initTournament() {
    globalHistory = 0; // Reset global history

    // Calculate masks
    pcIndexMask = (1 << pcIndexBits) - 1;
    localHistoryMask = (1 << lhistoryBits) - 1;
    globalHistoryMask = (1 << ghistoryBits) - 1;
  
    // Allocate and initialize Local PHT
    localPatternHistoryTable = (uint32_t*) malloc(sizeof(uint32_t) * (1 << pcIndexBits));
    for (int i = 0; i < (1 << pcIndexBits); i++) {
        localPatternHistoryTable[i] = 1; // Initialize to weakly taken
    }

    // Allocate and initialize Local BHT
    localBranchHistoryTable = (uint32_t*) malloc(sizeof(uint32_t) * (1 << lhistoryBits));
    for (int i = 0; i < (1 << lhistoryBits); i++) {
        localBranchHistoryTable[i] = 0; // Initialize to strongly not taken
    }

    // Allocate and initialize Global BHT
    globalBranchHistoryTable = (uint32_t*) malloc(sizeof(uint32_t) * (1 << ghistoryBits));
    for (int i = 0; i < (1 << ghistoryBits); i++) {
        globalBranchHistoryTable[i] = 1; // Initialize to weakly taken
    }

    // Allocate and initialize Chooser Table
    chooserTable = (uint32_t*) malloc(sizeof(uint32_t) * (1 << ghistoryBits));
    for (int i = 0; i < (1 << ghistoryBits); i++) {
        chooserTable[i] = 2; // Initialize to prefer global prediction
    }
}

// Predict the outcome using the tournament predictor
uint8_t tournamentPredict(uint32_t pc) {
    uint32_t pcIndex = pc & pcIndexMask;
    uint32_t globalHistoryIndex = globalHistory & globalHistoryMask;
    uint32_t choice = chooserTable[globalHistoryIndex];

    uint32_t prediction;
    if (choice < 2) {
        uint32_t localHistory = localPatternHistoryTable[pcIndex] & localHistoryMask;
        prediction = localBranchHistoryTable[localHistory];
    } else {
        prediction = globalBranchHistoryTable[globalHistoryIndex];
    }

    if (prediction > 1) {
        return TAKEN;
    } else {
        return NOTTAKEN;
    }
}

// Helper function to update prediction state
void updatePrediction(uint32_t *prediction, uint8_t outcome) {
    if (outcome == TAKEN) {
        if (*prediction < 3) (*prediction)++;
    } else {
        if (*prediction > 0) (*prediction)--;
    }
}

// Train the tournament predictor
void trainTournament(uint32_t pc, uint8_t outcome) {
    uint32_t pcIndex = pc & pcIndexMask;
    uint32_t globalHistoryIndex = globalHistory & globalHistoryMask;
    uint32_t choice = chooserTable[globalHistoryIndex];

    uint32_t localHistory = localPatternHistoryTable[pcIndex] & localHistoryMask;
    uint8_t localPrediction;
    if (localBranchHistoryTable[localHistory] > 1) {
        localPrediction = TAKEN;
    } else {
        localPrediction = NOTTAKEN;
    }

    uint8_t globalPrediction;
    if (globalBranchHistoryTable[globalHistoryIndex] > 1) {
        globalPrediction = TAKEN;
    } else {
        globalPrediction = NOTTAKEN;
    }

    // Update chooser table
    if (globalPrediction == outcome && localPrediction != outcome && chooserTable[globalHistoryIndex] < 3) {
        chooserTable[globalHistoryIndex]++;
    } else if (globalPrediction != outcome && localPrediction == outcome && chooserTable[globalHistoryIndex] > 0) {
        chooserTable[globalHistoryIndex]--;
    }

    // Update predictions
    updatePrediction(&globalBranchHistoryTable[globalHistoryIndex], outcome);
    updatePrediction(&localBranchHistoryTable[localHistory], outcome);

    // Update history tables
    localPatternHistoryTable[pcIndex] = ((localPatternHistoryTable[pcIndex] << 1) | outcome) & localHistoryMask;
    globalHistory = ((globalHistory << 1) | outcome) & globalHistoryMask;
}


//------------------------------------//
//      Perceptron Predictor Configuration       //
//------------------------------------//

int numPerceptrons = 220;
int perceptronHistoryLength = 30;
int perceptronTrainThreshold;
int16_t **perceptronTable;

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the perceptron predictor
void initPerceptron() {
    perceptronTable = (int16_t**)malloc(numPerceptrons * sizeof(int16_t*));
    for (int i = 0; i < numPerceptrons; i++) {
        perceptronTable[i] = (int16_t*)malloc((perceptronHistoryLength + 1) * sizeof(int16_t));
        for (int j = 0; j <= perceptronHistoryLength; j++) {
            perceptronTable[i][j] = 0; // Initialize weights to 0
        }
    }
    perceptronTrainThreshold = 60; // Simplified threshold calculation for illustration
    globalHistory = 0; // Reset global history
}


// Make a prediction using the perceptron predictor
uint8_t perceptronPredict(uint32_t pc) {
    uint32_t perceptronIndex = pc % numPerceptrons;
    int16_t y = perceptronTable[perceptronIndex][0]; // Start with the bias weight
    uint64_t currentHistory = globalHistory;
    for (int i = 1; i <= perceptronHistoryLength; i++) {
        if (currentHistory & 1) {
            y += perceptronTable[perceptronIndex][i];
        } else {
            y -= perceptronTable[perceptronIndex][i];
        }
        currentHistory >>= 1;
    }
    if (y < 0) {
        return NOTTAKEN;
    } else {
        return TAKEN;
    }
}


// Train the perceptron predictor
void trainPerceptron(uint32_t pc, uint8_t outcome) {
    uint32_t perceptronIndex = pc % numPerceptrons;
    int16_t y = perceptronTable[perceptronIndex][0];
    uint64_t currentHistory = globalHistory;
    for (int i = 1; i <= perceptronHistoryLength; i++) {
        if (currentHistory & 1) {
            y += perceptronTable[perceptronIndex][i];
        } else {
            y -= perceptronTable[perceptronIndex][i];
        }
        currentHistory >>= 1;
    }
    uint8_t bpResult;
    if (y < 0) {
        bpResult = NOTTAKEN;
    } else {
        bpResult = TAKEN;
    }
  
    bool mispredict = (bpResult != outcome);
    if (mispredict || abs(y) <= perceptronTrainThreshold) {
        if (outcome == TAKEN) {
            perceptronTable[perceptronIndex][0] += 1; // Update bias weight
        } else {
            perceptronTable[perceptronIndex][0] -= 1; // Update bias weight
        }
        currentHistory = globalHistory;
        for (int i = 1; i <= perceptronHistoryLength; i++) {
            if (outcome == (currentHistory & 1)) {
                if (abs(perceptronTable[perceptronIndex][i] + 1) < perceptronTrainThreshold) {
                    perceptronTable[perceptronIndex][i] += 1;
                }
            } else {
                if (abs(perceptronTable[perceptronIndex][i] - 1) < perceptronTrainThreshold) {
                    perceptronTable[perceptronIndex][i] -= 1;
                }
            }
            currentHistory >>= 1;
        }
    }
    globalHistory = ((globalHistory << 1) | outcome) & ((1 << perceptronHistoryLength) - 1); // Update global history
}


// Cleanup the perceptron predictor
void cleanupPerceptron() {
    free(perceptronTable);
}

void init_predictor()
{
  switch (bpType) {
    case STATIC:
    case GSHARE:
      initGShare();
      break;
    case TOURNAMENT:
      initTournament();
      break;
    case CUSTOM:
      initPerceptron();
      break;
    default:
      break;
  }
  
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t make_prediction(uint32_t pc)
{

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
      return gsharePredict(pc);
    case TOURNAMENT:
      return tournamentPredict(pc);
    case CUSTOM:
      return perceptronPredict(pc);
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//

void train_predictor(uint32_t pc, uint8_t outcome)
{

  switch (bpType) {
    case STATIC:
    case GSHARE:
      return trainGShare(pc, outcome);
    case TOURNAMENT:
      return trainTournament(pc, outcome);
    case CUSTOM:
      return trainPerceptron(pc, outcome);
    default:
      break;
  }
  

}