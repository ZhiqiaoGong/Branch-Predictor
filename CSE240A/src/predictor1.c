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

// //custom
// uint32_t *customGPHT; // Custom Global Pattern History Table
// uint32_t *customPCPHT; // Custom PC-Indexed Pattern History Table
// uint32_t *customASS; // Custom Adaptive Strategy Selector
// uint32_t *customBOHT; // Custom Branch Outcome History Table

// // Define maximum confidence level for a 3-bit confidence counter
// #define MAX_CONFIDENCE 7
// // Define the threshold above which a prediction is considered high confidence
// #define HIGH_CONFIDENCE_THRESHOLD 4



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
        default: printf("Undefined"); return NOTTAKEN;
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

    return prediction > 1 ? TAKEN : NOTTAKEN;
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
    uint8_t localPrediction = localBranchHistoryTable[localHistory] > 1 ? TAKEN : NOTTAKEN;
    uint8_t globalPrediction = globalBranchHistoryTable[globalHistoryIndex] > 1 ? TAKEN : NOTTAKEN;

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




/*
// custom
#include <math.h> // For floor and pow functions

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Define the size of the global history and the number of perceptrons
// const int ghistoryBits = 10; // Global History Register Bits
const int numPerceptrons = 512; // Number of perceptrons
const int weightBits = 8; // Number of bits for each weight

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

int16_t **perceptronTable; // 2D array for perceptrons and their weights
uint64_t globalHistory; // Global History Register
int threshold; // Threshold for training the perceptrons

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the Perceptron predictor
void initPerceptron() {
    threshold = (int)(floor(1.93 * ghistoryBits) + 14);
    globalHistory = 0;

    // Allocate memory for perceptron table
    perceptronTable = (int16_t**)malloc(numPerceptrons * sizeof(int16_t*));
    for (int i = 0; i < numPerceptrons; i++) {
        perceptronTable[i] = (int16_t*)malloc((ghistoryBits + 1) * sizeof(int16_t));
        for (int j = 0; j <= ghistoryBits; j++) {
            perceptronTable[i][j] = 0; // Initialize weights to 0
        }
    }
}

// Perceptron prediction function
uint8_t perceptronPredict(uint32_t pc) {
    uint32_t index = pc % numPerceptrons;
    int16_t *weights = perceptronTable[index];
    int y = weights[0]; // Bias weight

    for (int i = 1; i <= ghistoryBits; i++) {
        int bit = (globalHistory >> (i - 1)) & 1;
        if (bit == 1) y += weights[i];
        else y -= weights[i];
    }

    return (y >= 0) ? TAKEN : NOTTAKEN;
}

// Train the Perceptron predictor
// void trainPerceptron(uint32_t pc, uint8_t outcome) {
//     uint32_t index = pc % numPerceptrons;
//     int16_t *weights = perceptronTable[index];
//     int y = weights[0]; // Bias weight

//     for (int i = 1; i <= ghistoryBits; i++) {
//         int bit = (globalHistory >> (i - 1)) & 1;
//         if (bit == 1) y += weights[i];
//         else y -= weights[i];
//     }

//     if (((y >= 0) != outcome) || (abs(y) <= threshold)) {
//         weights[0] += (outcome == TAKEN) ? 1 : -1;
//         for (int i = 1; i <= ghistoryBits; i++) {
//             int bit = (globalHistory >> (i - 1)) & 1;
//             if ((bit == outcome) && (weights[i] < pow(2, weightBits - 1) - 1)) weights[i]++;
//             else if ((bit != outcome) && (weights[i] > -pow(2, weightBits - 1))) weights[i]--;
//         }
//     }

//     globalHistory = ((globalHistory << 1) | outcome) & ((1ULL << ghistoryBits) - 1);
// }
// Adjust weights and potentially adjust threshold
void trainPerceptron(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc % numPerceptrons;
    int16_t *weights = perceptronTable[index];
    int y = weights[0]; // Start with the bias weight

    for (int i = 1; i <= ghistoryBits; i++) {
        int bit = (globalHistory >> (i - 1)) & 1;
        y += (bit == 1) ? weights[i] : -weights[i];
    }

    // Prediction was incorrect or confidence is low
    if (((y >= 0) != outcome) || (abs(y) <= threshold)) {
        // Adjust threshold dynamically based on prediction accuracy
        threshold += ((y >= 0) != outcome) ? 1 : -1;
        threshold = threshold < 0 ? 0 : threshold; // Ensure threshold doesn't go negative

        // Adjust the bias weight
        weights[0] += (outcome == TAKEN) ? 1 : -1;

        // Adjust other weights
        for (int i = 1; i <= ghistoryBits; i++) {
            int bit = (globalHistory >> (i - 1)) & 1;
            if ((bit == outcome) && (weights[i] < pow(2, weightBits - 1) - 1)) weights[i]++;
            else if ((bit != outcome) && (weights[i] > -pow(2, weightBits - 1))) weights[i]--;
        }
    }

    // Update global history
    globalHistory = ((globalHistory << 1) | outcome) & ((1ULL << ghistoryBits) - 1);
}


// Cleanup the Perceptron predictor
void cleanupPerceptron() {
    for (int i = 0; i < numPerceptrons; i++) {
        free(perceptronTable[i]);
    }
    free(perceptronTable);
}*/

//------------------------------------//
//      Predictor Configuration       //
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
        y += (currentHistory & 1) ? perceptronTable[perceptronIndex][i] : -perceptronTable[perceptronIndex][i];
        currentHistory >>= 1;
    }
    return y < 0 ? NOTTAKEN : TAKEN;
}


// Train the perceptron predictor
void trainPerceptron(uint32_t pc, uint8_t outcome) {
    uint32_t perceptronIndex = pc % numPerceptrons;
    int16_t y = perceptronTable[perceptronIndex][0];
    uint64_t currentHistory = globalHistory;
    for (int i = 1; i <= perceptronHistoryLength; i++) {
        y += (currentHistory & 1) ? perceptronTable[perceptronIndex][i] : -perceptronTable[perceptronIndex][i];
        currentHistory >>= 1;
    }
    uint8_t bpResult = y < 0 ? NOTTAKEN : TAKEN;
  
    bool mispredict = bpResult != outcome;
    if (mispredict || abs(y) <= perceptronTrainThreshold) {
        perceptronTable[perceptronIndex][0] += (outcome == TAKEN) ? 1 : -1; // Update bias weight
        currentHistory = globalHistory;
        for (int i = 1; i <= perceptronHistoryLength; i++) {
            if ((outcome == (currentHistory & 1)) && abs(perceptronTable[perceptronIndex][i] + 1) < perceptronTrainThreshold) {
                perceptronTable[perceptronIndex][i] += 1;
            } else if (abs(perceptronTable[perceptronIndex][i] - 1) < perceptronTrainThreshold) {
                perceptronTable[perceptronIndex][i] -= 1;
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
      //init_tournament_gshare();
      //init_custom();
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
      //return tournament_gshare_predict(pc);
      //return custom_predict(pc);
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
      //return train_tournament_gshare(pc, outcome);
      //return train_custom(pc, outcome);
      return trainPerceptron(pc, outcome);
    default:
      break;
  }
  

}