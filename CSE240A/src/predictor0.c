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
// const char *studentName = "NAME";
// const char *studentID   = "PID";
// const char *email       = "EMAIL";

// //------------------------------------//
// //      Predictor Configuration       //
// //------------------------------------//

// // Handy Global for use in output routines
// const char *bpName[4] = { "Static", "Gshare",
//                           "Tournament", "Custom" };

// int ghistoryBits; // Number of bits used for Global History
// int lhistoryBits; // Number of bits used for Local History
// int pcIndexBits;  // Number of bits used for PC index
// int bpType;       // Branch Prediction Type
// int verbose;

// //------------------------------------//
// //      Predictor Data Structures     //
// //------------------------------------//

// //
// //TODO: Add your own Branch Predictor data structures here
// //


// //------------------------------------//
// //        Predictor Functions         //
// //------------------------------------//

// // Initialize the predictor
// //
// void
// init_predictor()
// {
//   //
//   //TODO: Initialize Branch Predictor Data Structures
//   //
// }

// // Make a prediction for conditional branch instruction at PC 'pc'
// // Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// // indicates a prediction of not taken
// //
// uint8_t
// make_prediction(uint32_t pc)
// {
//   //
//   //TODO: Implement prediction scheme
//   //

//   // Make a prediction based on the bpType
//   switch (bpType) {
//     case STATIC:
//       return TAKEN;
//     case GSHARE:
//     case TOURNAMENT:
//     case CUSTOM:
//     default:
//       break;
//   }

//   // If there is not a compatable bpType then return NOTTAKEN
//   return NOTTAKEN;
// }

// // Train the predictor the last executed branch at PC 'pc' and with
// // outcome 'outcome' (true indicates that the branch was taken, false
// // indicates that the branch was not taken)
// //
// void
// train_predictor(uint32_t pc, uint8_t outcome)
// {
//   //
//   //TODO: Implement Predictor training
//   //
// }


// //========================================================//
// //  predictor.c                                           //
// //  Source file for the Branch Predictor                  //
// //                                                        //
// //  Implement the various branch predictors below as      //
// //  described in the README                               //
// //========================================================//
// #include <stdio.h>
// #include <math.h>
// #include "predictor.h"


// //
// // TODO:Student Information
// //
// const char *studentName = "Gandhar Deshpande";
// const char *studentID   = "A59005457";
// const char *email       = "gdeshpande@ucsd.edu";

// //------------------------------------//
// //      Predictor Configuration       //
// //------------------------------------//

// // Handy Global for use in output routines
// const char *bpName[4] = { "Static", "Gshare",
//                           "Tournament", "Custom" };

// //define number of bits required for indexing the BHT here. 
// int ghistoryBits = 14; // Number of bits used for Global History
// int bpType;       // Branch Prediction Type
// int verbose;

// int lhistoryBits; // Number of bits used for Local History
// int pcIndexBits;  // Number of bits used for PC index


// //------------------------------------//
// //      Predictor Data Structures     //
// //------------------------------------//

// //
// //TODO: Add your own Branch Predictor data structures here
// //
// //gshare
// uint8_t *bht_gshare;
// uint64_t ghistory;


// //------------------------------------//
// //        Predictor Functions         //
// //------------------------------------//

// // Initialize the predictor
// //

// //gshare functions
// void init_gshare() {
//  int bht_entries = 1 << ghistoryBits;
//   bht_gshare = (uint8_t*)malloc(bht_entries * sizeof(uint8_t));
//   int i = 0;
//   for(i = 0; i< bht_entries; i++){
//     bht_gshare[i] = WN;
//   }
//   ghistory = 0;
// }



// uint8_t 
// gshare_predict(uint32_t pc) {
//   //get lower ghistoryBits of pc
//   uint32_t bht_entries = 1 << ghistoryBits;
//   uint32_t pc_lower_bits = pc & (bht_entries-1);
//   uint32_t ghistory_lower_bits = ghistory & (bht_entries -1);
//   uint32_t index = pc_lower_bits ^ ghistory_lower_bits;
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

// void
// train_gshare(uint32_t pc, uint8_t outcome) {
//   //get lower ghistoryBits of pc
//   uint32_t bht_entries = 1 << ghistoryBits;
//   uint32_t pc_lower_bits = pc & (bht_entries-1);
//   uint32_t ghistory_lower_bits = ghistory & (bht_entries -1);
//   uint32_t index = pc_lower_bits ^ ghistory_lower_bits;

//   //Update state of entry in bht based on outcome
//   switch(bht_gshare[index]){
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

// void
// cleanup_gshare() {
//   free(bht_gshare);
// }



// void
// init_predictor()
// {
//   switch (bpType) {
//     case STATIC:
//     case GSHARE:
//       init_gshare();
//       break;
//     case TOURNAMENT:
//     case CUSTOM:
//     default:
//       break;
//   }
  
// }

// // Make a prediction for conditional branch instruction at PC 'pc'
// // Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// // indicates a prediction of not taken
// //
// uint8_t
// make_prediction(uint32_t pc)
// {

//   // Make a prediction based on the bpType
//   switch (bpType) {
//     case STATIC:
//       return TAKEN;
//     case GSHARE:
//       return gshare_predict(pc);
//     case TOURNAMENT:
//     case CUSTOM:
//     default:
//       break;
//   }

//   // If there is not a compatable bpType then return NOTTAKEN
//   return NOTTAKEN;
// }

// // Train the predictor the last executed branch at PC 'pc' and with
// // outcome 'outcome' (true indicates that the branch was taken, false
// // indicates that the branch was not taken)
// //

// void
// train_predictor(uint32_t pc, uint8_t outcome)
// {

//   switch (bpType) {
//     case STATIC:
//     case GSHARE:
//       return train_gshare(pc, outcome);
//     case TOURNAMENT:
//     case CUSTOM:
//     default:
//       break;
//   }
  

// }

// https://github.com/will-zegers/240a_branch_prediction/tree/master
// //#include "math.h"

// #include <stdbool.h>
// #define TAKEN     true
// #define NOT_TAKEN false
// #define BUDGET    32768 // (32Kb)

// int bpType;
// int ghistoryBits;
// int lhistoryBits;
// int pcIndexBits;
// int verbose;


// const static int h = 31;

// //const static float theta = 1.93 * h + 14;
// //const static int n = pow(2,floor(log2(BUDGET/(h * 8))));
// const static float theta = h/2; 
// const static int n = 128;

// char W[n][h+1];
// int H;

// int index;
// int y;

// void init_predictor ()
// {
//     for(int i = 0; i < n; i++)
//         for(int j = 0; j < h; j++)
//             W[i][j] = 0;

//     H = 0;
// }

// bool make_prediction (unsigned int pc)
// {
//     index = pc % n;

//     y = W[index][0];
//     for(int i = 1; i < h+1; i++)
//     {
//         y += W[index][i] * (H >> (h - i) & 1); 
//     }

//     return (y > 0) ? TAKEN : NOT_TAKEN;
// }

// void train_predictor (unsigned int pc, bool outcome)
// {
//     int t = (outcome == TAKEN) ? 1 : -1;

//     if(((y < 0) != (t < 0)) || (y < theta && y > -theta))
//     {
//         W[index][0] += t;
//         for(int i = 1; i < h+1; i++)
//         {
//             W[index][i] += (H >> (h - i) & 1)*t;
//         }           
//     }
//     H <<= 1;
//     H += (int)outcome;
// }

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
int ghistoryBits = 12; // Number of bits used for Global History
int bpType;       // Branch Prediction Type
int verbose;

int pcIndexBits = 10;  // Number of bits used for PC index, determine the size of the pattern history table (PHT)
int lhistoryBits = 10; // Number of bits used for Local History, determine the size of the branch history table (BHT)


int ghistoryBits_custom = 12; // Number of bits used for Global History, determine the size of the Global branch history table (BHT)
int pcIndexBits_custom = 10;  // Number of bits used for PC index, determine the size of the pattern history table (PHT)
int lhistoryBits_custom = 11; // Number of bits used for Local History, determine the size of the branch history table (BHT)


//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
//TODO: Add your own Branch Predictor data structures here
//
//gshare
uint8_t *bht_gshare;
uint64_t ghistory;

//tournament
uint32_t *local_pht;
uint32_t *local_bht;
uint32_t *global_bht;
uint32_t *choice_bht;

uint32_t gmask;
uint32_t lmask;
uint32_t pcmask;


//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//

//gshare functions
void init_gshare() {
 int bht_entries = 1 << ghistoryBits;
  bht_gshare = (uint8_t*)malloc(bht_entries * sizeof(uint8_t));
  int i = 0;
  for(i = 0; i< bht_entries; i++){
    bht_gshare[i] = WN;
  }
  ghistory = 0;
}


uint8_t 
gshare_predict(uint32_t pc) {
  //get lower ghistoryBits of pc
  uint32_t bht_entries = 1 << ghistoryBits;
  uint32_t pc_lower_bits = pc & (bht_entries-1);
  uint32_t ghistory_lower_bits = ghistory & (bht_entries -1);
  uint32_t index = pc_lower_bits ^ ghistory_lower_bits;
  switch(bht_gshare[index]){
    case WN:
      return NOTTAKEN;
    case SN:
      return NOTTAKEN;
    case WT:
      return TAKEN;
    case ST:
      return TAKEN;
    default:
      printf("Warning: Undefined state of entry in GSHARE BHT!\n");
      return NOTTAKEN;
  }
}

void train_gshare(uint32_t pc, uint8_t outcome) {
  //get lower ghistoryBits of pc
  uint32_t bht_entries = 1 << ghistoryBits;
  uint32_t pc_lower_bits = pc & (bht_entries-1);
  uint32_t ghistory_lower_bits = ghistory & (bht_entries -1);
  uint32_t index = pc_lower_bits ^ ghistory_lower_bits;

  //Update state of entry in bht based on outcome
  switch(bht_gshare[index]){
    case WN:
      bht_gshare[index] = (outcome==TAKEN)?WT:SN;
      break;
    case SN:
      bht_gshare[index] = (outcome==TAKEN)?WN:SN;
      break;
    case WT:
      bht_gshare[index] = (outcome==TAKEN)?ST:WN;
      break;
    case ST:
      bht_gshare[index] = (outcome==TAKEN)?ST:WT;
      break;
    default:
      printf("Warning: Undefined state of entry in GSHARE BHT!\n");
  }

  //Update history register
  ghistory = ((ghistory << 1) | outcome); 
}

void cleanup_gshare() {
  free(bht_gshare);
}




//Tournament initilization function
void init_tournament() {
  ghistory = 0;
  int size;

  // make a mask for pc, local_pht, ghistory, 
  uint32_t pht_entries = 1 << pcIndexBits;
  pcmask = pht_entries-1;

  //get lower ghistoryBits
  uint32_t mask = 1 << lhistoryBits;
  lmask = mask -1;

  uint32_t bht_entries = 1 << ghistoryBits;
  gmask = bht_entries -1;
  
  
  // Local PHT
  size = 1<<pcIndexBits;
  local_pht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    local_pht[i] = 1;
  }
  // Local BHT
  size = 1<<lhistoryBits;
  local_bht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    local_bht[i] = 0;
  }
  // Global PHT
  size = 1<<ghistoryBits;
  global_bht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    global_bht[i] = 1;
  }

  // Choice PHT
  size = 1<<ghistoryBits;
  choice_bht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    choice_bht[i] = 2;
  }
  
}


uint8_t tournament_predict(uint32_t pc) {
  
  //tournament
  uint32_t choice;
  uint32_t lhist;
  uint32_t prediction;

  //get lower pht index from pc
  uint32_t pc_lower_bits = pc & pcmask;

  //get ghistoryBits
  uint32_t ghistbits = ghistory & gmask;
  choice = choice_bht[ghistbits];
  
  if(choice < 2)
  {
    lhist = lmask & local_pht[pc_lower_bits];
    prediction = local_bht[lhist];
  }
  else
  {
    prediction = global_bht[ghistbits];
  }

  if(prediction > 1){
    return TAKEN;
  }
  else{
    return NOTTAKEN;
  }

}


void train_tournament(uint32_t pc, uint8_t outcome) {

  // tournament
  uint32_t ghistbits;
  uint32_t choice;
  uint32_t pcidx;
  uint32_t lhist;
  uint32_t lpred;
  uint32_t gpred;

  //printf("Global Mask : %u\nLocal Mask : %u\nPC Mask : %u\n",gmask,lmask,pcmask);

  //get lower pht index from pc
  uint32_t pc_lower_bits = pc & pcmask;

  //get ghistoryBits
  ghistbits = ghistory & gmask;
  choice = choice_bht[ghistbits];

  lhist = lmask & local_pht[pc_lower_bits];
  lpred = local_bht[lhist];
  if(lpred>1)
    lpred = TAKEN;
  else
    lpred = NOTTAKEN;
  gpred = global_bht[ghistbits];
  if(gpred>1)
    gpred = TAKEN;
  else
    gpred = NOTTAKEN;

  if(gpred==outcome && lpred!=outcome && choice_bht[ghistbits]!=3)
    choice_bht[ghistbits]++;
  else if(gpred!=outcome && lpred==outcome && choice_bht[ghistbits]!=0)
    choice_bht[ghistbits]--;
  if(outcome==TAKEN)
  {
    if(global_bht[ghistbits]!=3)
      global_bht[ghistbits]++;
    if(local_bht[lhist]!=3)
      local_bht[lhist]++;
  }
  else
  {
    if(global_bht[ghistbits]!=0)
      global_bht[ghistbits]--;
    if(local_bht[lhist]!=0)
      local_bht[lhist]--;
  }
  local_pht[pc_lower_bits] = ((local_pht[pc_lower_bits]<<1) | outcome) & lmask;
  ghistory = ((ghistory<<1) | outcome) & gmask;

}






//========================================================//
//  CUSTOM 
//========================================================//

//tournament_gshare initilization function
void init_tournament_gshare() {
  ghistory = 0;
  int size;

  // make a mask for pc, local_pht, ghistory, 
  uint32_t pht_entries = 1 << pcIndexBits_custom;
  pcmask = pht_entries-1;

  //get lower ghistoryBits
  uint32_t mask = 1 << lhistoryBits_custom;
  lmask = mask -1;

  uint32_t bht_entries = 1 << ghistoryBits_custom;
  gmask = bht_entries -1;
  
  
  // Local PHT
  size = 1<<pcIndexBits;
  local_pht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    local_pht[i] = 1;
  }
  // Local BHT
  size = 1<<lhistoryBits;
  local_bht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    local_bht[i] = 0;
  }
  // Global PHT
  size = 1<<ghistoryBits;
  global_bht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    global_bht[i] = 1;
  }

  // Choice PHT
  size = 1<<ghistoryBits;
  choice_bht = (uint32_t*) malloc(sizeof(uint32_t)*size);
  for(int i=0;i<size;i++)
  {
    choice_bht[i] = 2;
  }
  
}


uint8_t tournament_gshare_predict(uint32_t pc) {
  
  //tournament
  uint32_t choice;
  uint32_t lhist;
  uint32_t prediction;

  //get lower pht index from pc
  uint32_t pc_lower_bits = pc & pcmask;

  //get ghistoryBits and global_bht_index
  uint32_t ghistbits = ghistory & gmask;
  uint32_t pcindex = pc & gmask;
  uint32_t global_bht_index = pcindex ^ ghistbits;
  choice = choice_bht[global_bht_index];
  
  if(choice < 2)
  {
    lhist = lmask & local_pht[pc_lower_bits];
    prediction = local_bht[lhist];
  }
  else
  {
    prediction = global_bht[global_bht_index];
  }

  if(prediction > 1){
    return TAKEN;
  }
  else{
    return NOTTAKEN;
  }

}


void train_tournament_gshare(uint32_t pc, uint8_t outcome) {

  // tournament
  uint32_t choice;
  uint32_t lhist;
  uint32_t lpred;
  uint32_t gpred;

  //printf("Global Mask : %u\nLocal Mask : %u\nPC Mask : %u\n",gmask,lmask,pcmask);

  //get lower pht index from pc
  uint32_t pc_lower_bits = pc & pcmask;

  //get ghistoryBits
  uint32_t ghistbits = ghistory & gmask;
  uint32_t pcindex = pc & gmask;
  uint32_t global_bht_index = pcindex ^ ghistbits;

  choice = choice_bht[global_bht_index];

  lhist = lmask & local_pht[pc_lower_bits];
  lpred = local_bht[lhist];
  if(lpred>1)
    lpred = TAKEN;
  else
    lpred = NOTTAKEN;
  gpred = global_bht[global_bht_index];
  if(gpred>1)
    gpred = TAKEN;
  else
    gpred = NOTTAKEN;

  if(gpred==outcome && lpred!=outcome && choice_bht[global_bht_index]!=3)
    choice_bht[global_bht_index]++;
  else if(gpred!=outcome && lpred==outcome && choice_bht[global_bht_index]!=0)
    choice_bht[global_bht_index]--;
  if(outcome==TAKEN)
  {
    if(global_bht[global_bht_index]!=3)
      global_bht[global_bht_index]++;
    if(local_bht[lhist]!=3)
      local_bht[lhist]++;
  }
  else
  {
    if(global_bht[global_bht_index]!=0)
      global_bht[global_bht_index]--;
    if(local_bht[lhist]!=0)
      local_bht[lhist]--;
  }
  local_pht[pc_lower_bits] = ((local_pht[pc_lower_bits]<<1) | outcome) & lmask;
  ghistory = ((ghistory<<1) | outcome) & gmask;

}

// custom: Multi-Level Adaptive Prediction
// Global variables for CUSTOM predictor
uint8_t *accuracy_history; // Example: A simple counter to track accuracy
uint64_t custom_ghistory; // You might want different history lengths for custom

void init_custom() {
  // Initialize your custom predictor's data structures here
  // This is just a placeholder for whatever setup you need
  custom_ghistory = 0;
  //accuracy_history = (uint8_t*)malloc(sizeof(uint8_t) * 2); // 0: gshare, 1: tournament
  accuracy_history = (uint8_t*)malloc(sizeof(uint8_t) * 2);
  if (accuracy_history == NULL) {
      fprintf(stderr, "Failed to allocate memory for accuracy_history\n");
      exit(1); // or handle the error in a more graceful way
  }

  memset(accuracy_history, 0, sizeof(uint8_t) * 2);
}

uint8_t custom_predict(uint32_t pc) {
  // Here, decide whether to use gshare's or tournament's prediction
  // This could be based on recent accuracy, a heuristic, or another method
  // For simplicity, let's say we just toggle between them based on which was recently more accurate

  uint8_t gshare_pred = gshare_predict(pc);
  uint8_t tournament_pred = tournament_predict(pc);

  // Placeholder for decision logic; you would have your own
  // For now, let's just assume we prefer the predictor that was last accurate
  if (accuracy_history[0] >= accuracy_history[1]) {
    return gshare_pred;
  } else {
    return tournament_pred;
  }
}

// Returns the confidence level of gshare's prediction: 0 (low) or 1 (high)
uint8_t get_gshare_confidence(uint32_t pc) {
    uint32_t index = (pc ^ ghistory) & ((1 << ghistoryBits) - 1);
    uint8_t state = bht_gshare[index];
    // High confidence for strongly taken (ST) or strongly not taken (SN) states
    return (state == ST || state == SN) ? 1 : 0;
}


// Returns the confidence level of tournament's prediction: 0 (low) or 1 (high)
uint8_t get_tournament_confidence(uint32_t pc) {
    uint32_t ghistbits = ghistory & ((1 << ghistoryBits) - 1);
    uint8_t choice_state = choice_bht[ghistbits];
    // Assuming higher values indicate more confidence in the global predictor
    // and lower values more confidence in the local predictor.
    // For simplicity, we consider extreme values as high confidence.
    return (choice_state == 0 || choice_state == 3) ? 1 : 0;
}



// void train_custom(uint32_t pc, uint8_t outcome) {
//     // Train both predictors
//     train_gshare(pc, outcome);
//     train_tournament(pc, outcome);

//     // Evaluate prediction and confidence
//     uint8_t gshare_pred = gshare_predict(pc);
//     uint8_t tournament_pred = tournament_predict(pc);
//     uint8_t gshare_confidence = get_gshare_confidence(pc);
//     uint8_t tournament_confidence = get_tournament_confidence(pc);

//     // Example strategy: if both predictors agree, no need to adjust confidence
//     // If they disagree, increase confidence in the one that was correct
//     if (gshare_pred != tournament_pred) {
//         if (gshare_pred == outcome) {
//             accuracy_history[0] += gshare_confidence ? 2 : 1; // More weight if confident
//         } else {
//             accuracy_history[1] += tournament_confidence ? 2 : 1; // More weight if confident
//         }
//     }
// }

void train_custom(uint32_t pc, uint8_t outcome) {
  // Train both the gshare and tournament predictors
  train_gshare(pc, outcome);
  train_tournament(pc, outcome);

  // Update accuracy history based on which predictor was correct
  uint8_t gshare_pred = gshare_predict(pc);
  uint8_t tournament_pred = tournament_predict(pc);
  
  if (gshare_pred == outcome) {
    accuracy_history[0]++; // Increment accuracy for gshare
  }
  if (tournament_pred == outcome) {
    accuracy_history[1]++; // Increment accuracy for tournament
  }

  // Here, you could also implement logic to decrement or reset counters
  // to ensure the predictor stays adaptive over time
}

void cleanup_custom() {
    free(accuracy_history);
}






void init_predictor()
{
  switch (bpType) {
    case STATIC:
    case GSHARE:
      init_gshare();
      break;
    case TOURNAMENT:
      init_tournament();
      break;
    case CUSTOM:
      //init_tournament_gshare();
      init_custom();
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
      return gshare_predict(pc);
    case TOURNAMENT:
      return tournament_predict(pc);
    case CUSTOM:
      //return tournament_gshare_predict(pc);
      return custom_predict(pc);
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
      return train_gshare(pc, outcome);
    case TOURNAMENT:
      return train_tournament(pc, outcome);
    case CUSTOM:
      //return train_tournament_gshare(pc, outcome);
      return train_custom(pc, outcome);
    default:
      break;
  }
  

}