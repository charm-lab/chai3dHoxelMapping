% Hoxel Mapping Experiments (HME) Statistical Analysis
% Author: Jasmin E. Palmer
function runHMEStats()
% Needs to run hmeDataProcessing.m 1st and select 
% "Run Section" in order to pull data from workspace
%% Statistcal Tests v2 -- N-Way ANOVA
    % 2-way ANOVA
    % p = anovan(y, group)
    showStats = "on";
    format long; close all;
    fontSize = 10; width = 1700; height = 1000;
    
    interactableCompare = true;
    for p = 1:numExperimentTypes
        if (p == 1)
            % Groups:
            % Mapping groups
            mappingsExp1(1:...
                numSubjects*numTrialsPerMapping(p), 1) = {'1'};
            mappingsExp1(numSubjects*numTrialsPerMapping(p) + 1:...
                2*numSubjects*numTrialsPerMapping(p), 1) = {'3'};
            mappingsExp1(2*numSubjects*numTrialsPerMapping(p)+1:...
                3*numSubjects*numTrialsPerMapping(p),1) = {'5'};

            % Experiment Type condition
            experimentType1(1:3*numSubjects*numTrialsPerMapping(p), 1) = ...
                {'training'};

        else % (p == 2)
            % Groups:
            % Mapping groups
            mappingsExp2(1:...
                numSubjects*numTrialsPerMapping(p), 1) = {'1'};
            mappingsExp2(numSubjects*numTrialsPerMapping(p) + 1:...
                2*numSubjects*numTrialsPerMapping(p), 1) = {'3'};
            mappingsExp2(2*numSubjects*numTrialsPerMapping(p)+1:...
                3*numSubjects*numTrialsPerMapping(p),1) = {'5'};

            % Experiment Type condition
            experimentType2(1:3*numSubjects*numTrialsPerMapping(p), 1) = ...
                {'testing'};
        end
    end


    %Convert matrix of subject results to column vectors
    p=1;
    %CompletionTime Mapping 1 - Training
    yCT_Map1_Train = reshape([completionTimeMapping1{:,p}],[],1);
    %CompletionTime Mapping 3 - Training
    yCT_Map3_Train  = reshape([completionTimeMapping3{:,p}],[],1);
    %CompletionTime Mapping 5 - Training
    yCT_Map5_Train  = reshape([completionTimeMapping5{:,p}],[],1);
    p=2;
    %CompletionTime Mapping 1 - Testing
    yCT_Map1_Test = reshape([completionTimeMapping1{:,p}],[],1);
    %CompletionTime Mapping 3 - Testing
    yCT_Map3_Test  = reshape([completionTimeMapping3{:,p}],[],1);
    %CompletionTime Mapping 5 - Testing
    yCT_Map5_Test  = reshape([completionTimeMapping5{:,p}],[],1);
    
%     %IndexPathLength Mapping 1 Cube group
%     yIPL_Map1 = reshape(indexPathLengthMapping1(:,1:numSubjects),[],1);
%     %IndexPathLength Mapping 3 Cube group
%     yIPL_Map3 = reshape(indexPathLengthMapping3(:,1:numSubjects),[],1);
%     %IndexPathLength Mapping 5 Cube group
%     yIPL_Map5 = reshape(indexPathLengthMapping5(:,1:numSubjects),[],1);
%     
%     %ThumbPathLength Mapping 1 Cube group
%     yTPL_Map1 = reshape(thumbPathLengthMapping1(:,1:numSubjects),[],1);
%     %ThumbPathLength Mapping 3 Cube group
%     yTPL_Map3 = reshape(thumbPathLengthMapping3(:,1:numSubjects),[],1);
%     %ThumbPathLength Mapping 5 Cube group
%     yTPL_Map5 = reshape(thumbPathLengthMapping5(:,1:numSubjects),[],1);
%     
%     %BoxPathLength Mapping 1 Cube group
%     yBPL_Map1 = reshape(boxPathLengthMapping1(:,1:numSubjects),[],1);
%     %BoxPathLength Mapping 3 Cube group
%     yBPL_Map3 = reshape(boxPathLengthMapping3(:,1:numSubjects),[],1);
%     %BoxPathLength Mapping 5 Cube group
%     yBPL_Map5 = reshape(boxPathLengthMapping5(:,1:numSubjects),[],1);
%     
%     %IndexShearForceMag Mapping 1 Cube group
%     yISF_Map1 = reshape(meanIndexShearForceMapping1(:,1:numSubjects),[],1);
%     %IndexShearForceMag Mapping 3 Cube group
%     yISF_Map3 = reshape(meanIndexShearForceMapping3(:,1:numSubjects),[],1);
%     %IndexShearForceMag Mapping 5 Cube group
%     yISF_Map5 = reshape(meanIndexShearForceMapping5(:,1:numSubjects),[],1);
%     
%     %IndexNormalForceMag Mapping 1 Cube group
%     yINF_Map1 = reshape(meanIndexNormalForceMapping1(:,1:numSubjects),[],1);
%     %IndexNormalForceMag Mapping 3 Cube group
%     yINF_Map3 = reshape(meanIndexNormalForceMapping3(:,1:numSubjects),[],1);
%     %IndexNormalForceMag Mapping 5 Cube group
%     yINF_Map5 = reshape(meanIndexNormalForceMapping5(:,1:numSubjects),[],1);
%     
%     %ThumbShearForceMag Mapping 1 Cube group
%     yTSF_Map1 = reshape(meanThumbShearForceMapping1(:,1:numSubjects),[],1);
%     %ThumbShearForceMag Mapping 3 Cube group
%     yTSF_Map3 = reshape(meanThumbShearForceMapping3(:,1:numSubjects),[],1);
%     %ThumbShearForceMag Mapping 5 Cube group
%     yTSF_Map5 = reshape(meanThumbShearForceMapping5(:,1:numSubjects),[],1);
%     
%     %ThumbNormalForceMag Mapping 1 Cube group
%     yTNF_Map1 = reshape(meanThumbNormalForceMapping1(:,1:numSubjects),[],1);
%     %ThumbNormalForceMag Mapping 3 Cube group
%     yTNF_Map3 = reshape(meanThumbNormalForceMapping3(:,1:numSubjects),[],1);
%     %ThumbNormalForceMag Mapping 5 Cube group
%     yTNF_Map5 = reshape(meanThumbNormalForceMapping5(:,1:numSubjects),[],1);
    
    %Find p-values
    %vertically concatenate columns of the same metric
    yCT_Train = [yCT_Map1_Train; yCT_Map3_Train; yCT_Map5_Train];
    
    disp("Train")
    % [p_CompletionTime, ~, stats_CompletionTime] = ...
    %     anovan(yCT_Train, {mappingsExp1},...
    %     "Model","interaction", "Varnames",["mappings"],...
    %     "display", showStats)

     % [p_CompletionTime, ~, stats_CompletionTime] = ...
     aov = anova({mappingsExp1}, yCT_Train, FactorNames=["mappings"])


    yCT_Test = [yCT_Map1_Test; yCT_Map3_Test; yCT_Map5_Test];


    disp("Test")
    % [p_CompletionTime, ~, stats_CompletionTime] = ...
    %     anovan(yCT_Test, {mappingsExp2},...
    %     "Model","interaction", "Varnames",["mappings"],...
    %     "display", showStats)
    aov = anova({mappingsExp2}, yCT_Test, FactorNames=["mappings"])

    disp("Both")
    yCT = [yCT_Train; yCT_Test];
    mappings = [mappingsExp1; mappingsExp2];
    experimentType = [experimentType1; experimentType2];
    
    [p_CompletionTime, ~ , stats_CompletionTime] = ...
    anovan(yCT, {mappings, experimentType},...
    "Model","interaction", "Varnames",["mappings","experimentType"],...
    "display", showStats)

    %     yIPL = [yIPL_Map1; yIPL_Map3; yIPL_Map5];
%     [p_IndexPathLength, ~, stats_IndexPathLength] = anovan(yIPL, {mappings},...
%         "Model","interaction", "Varnames","mappings", "display",showStats);
% 
%     yTPL = [yTPL_Map1; yTPL_Map3; yTPL_Map5];
%     [p_ThumbPathLength, ~, stats_ThumbPathLength] = anovan(yTPL, {mappings},...
%         "Model","interaction", "Varnames","mappings","display",showStats);
% 
%     yBPL = [yBPL_Map1; yBPL_Map3; yBPL_Map5];
%     %     p_BoxPathLength = anovan(yBPL, {mappings})
%     [p_BoxPathLength, ~, stats_BoxPathLength] = anovan(yBPL, {mappings},...
%         "Model","interaction", "Varnames","mappings", "display",showStats);
%     
%     yISF = [yISF_Map1; yISF_Map3; yISF_Map5];
%     [p_IndexShearForce, ~, stats_IndexShearForce] = anovan(yISF, {mappings},...
%         "Model","interaction", "Varnames","mappings","display",showStats);
% 
%     yINF = [yINF_Map1; yINF_Map3; yINF_Map5];
%     [p_IndexNormalForce, ~, stats_IndexNormalForce] = anovan(yINF, {mappings},...
%         "Model","interaction", "Varnames", "mappings", "display",showStats);
% 
%     yTSF = [yTSF_Map1; yTSF_Map3; yTSF_Map5];
%     [p_ThumbShearForce, ~, stats_ThumbShearForce] = anovan(yTSF, {mappings},...
%         "Model","interaction", "Varnames", "mappings", "display",showStats);
% 
%     yTNF = [yTNF_Map1; yTNF_Map3; yTNF_Map5];
%     [p_ThumbNormalForce, ~, stats_ThumbNormalForce] = anovan(yTNF, {mappings},...
%         "Model","interaction", "Varnames", "mappings", "display",showStats);

    % Compare signifcance with control group only:
    if(interactableCompare == false)
        close all;
        figure;
        %Completion Time
        subplot(3,3,1)    
        compCT = multcompare(stats_CompletionTime, "CriticalValueType", "dunnett", ...
            "ControlGroup", 3, "Alpha", 0.05);
        title("Completion Time -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
    
        %Index Path Length   
        subplot(3,3,2)    
        compIPL = multcompare(stats_IndexPathLength, "CriticalValueType", "dunnett", ...
            "ControlGroup", 5, "Alpha", 0.05);
        title("Index Path Length -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
    
        %Thumb Path Length
        subplot(3,3,3)    
        compTPL = multcompare(stats_ThumbPathLength, "CriticalValueType", "dunnett", ...
            "ControlGroup", 5, "Alpha", 0.05);
        title("Thumb Path Length -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
    
        %Box Path Length
        subplot(3,3,4)    
        compBPL = multcompare(stats_BoxPathLength, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Box Path Length -- Control")
        improvePlot_v2(false, true, fontSize, width, height)    
    
        %Index Shear Force
        subplot(3,3,5)        
        compISF = multcompare(stats_IndexShearForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Index Shear Force -- Control")
        improvePlot_v2(false, true, fontSize, width, height)    
    
        %Index Normal Force
        subplot(3,3,6)    
        compINF = multcompare(stats_IndexNormalForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Index Normal Force -- Control")
        improvePlot_v2(false, true, fontSize, width, height)
        
        %Thumb Shear Force
        subplot(3,3,7)    
        compTSF = multcompare(stats_ThumbShearForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        title("Thumb Shear Force -- Control")
        improvePlot_v2(false, true, fontSize, width, height)           
    
        %Thumb Normal Force
        subplot(3,3,8)    
        compTNF = multcompare(stats_ThumbNormalForce, "CriticalValueType", ...
            "dunnett", "ControlGroup", 5, "Alpha", 0.05);
        improvePlot_v2(false, true, fontSize, width, height) 
        title("Thumb Normal Force -- Control")
        
    
        %Tables with Mapping 5 as Control:    
        tbl_CompletionTime = array2table(compCT,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexPathLength = array2table(compIPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbPathLength = array2table(compTPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_BoxPathLength = array2table(compBPL,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexShearForce = array2table(compISF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_IndexNormalForce = array2table(compINF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbShearForce = array2table(compTSF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
        tbl_ThumbNormalForce = array2table(compTNF,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
   
    % Interactable comparison with any group:
    else    
        close all;
        figure;
    
%         subplot(3,3,1)
        compCT = multcompare(stats_CompletionTime);
        title("Completion Time -- Interactable")
        improvePlot_v2(false, true, fontSize, width, height)
    
%         subplot(3,3,2)
%         compIPL = multcompare(stats_IndexPathLength);
%         title("Index Path Length -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
%     
%         subplot(3,3,3)
%         compTPL = multcompare(stats_ThumbPathLength);
%         title("Thumb Path Length -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
%     
%         subplot(3,3,4)
%         compBPL = multcompare(stats_BoxPathLength);
%         title("Box Path Length -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
%     
%         subplot(3,3,5)
%         compISF = multcompare(stats_IndexShearForce);
%         title("Index Shear Force -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
%     
%         subplot(3,3,6)
%         compINF = multcompare(stats_IndexNormalForce);
%         title("Index Normal Force -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
%     
%         subplot(3,3,7)
%         compTSF = multcompare(stats_ThumbShearForce);
%         title("Thumb Shear Force -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
%     
%         subplot(3,3,8)
%         compTNF = multcompare(stats_ThumbNormalForce);
%         title("Thumb Normal Force -- Interactable")
%         improvePlot_v2(false, true, fontSize, width, height)
    
        
        %Tables with Variable Control:    
        tbl_CompletionTime = array2table(compCT,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
    
%         tbl_IndexPathLength = array2table(compIPL,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
%         tbl_ThumbPathLength = array2table(compTPL,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
%         tbl_BoxPathLength = array2table(compBPL,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
%         tbl_IndexShearForce = array2table(compISF,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
%         tbl_IndexNormalForce = array2table(compINF,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
%         tbl_ThumbShearForce = array2table(compTSF,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
%         tbl_ThumbNormalForce = array2table(compTNF,"VariableNames", ...
%         ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"])
%     
    end

    if (showStats == "on")
        p_CompletionTime
        p_IndexPathLength
        p_ThumbPathLength
        p_BoxPathLength
        p_IndexShearForce
        p_IndexNormalForce
        p_ThumbShearForce
        p_ThumbNormalForce
%         p_BoxDrops
    end       
end