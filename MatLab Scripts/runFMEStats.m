% FingerMapping Experiments Statistical Analysis
function runFMEStats()
% Needs to run fingerMappingDataPlotting_v2.m 1st and select 
% "Run Section" in order to pull data from workspace

%% Statistcal Tests v1 -- 1-Way ANOVA Attempt
%     % H0 - There is *no* difference between the mean values by mapping
%     % H1 - There is a difference between the mean values by mapping
%     
%     % p = friedman(x,reps) returns p-value for the nonparametric Friedman's test to compare column
%     % effects in a two-way layout. friedman tests the null hypothesis
%     % that the column effects are all the same against the alternative
%     % that they are not all the same.
%     
%     % [ ' ] - Columns are visible vs invisible
%     % [   ] - Columns are by mapping
%     
%     
%     disp("ANOVA by Mapping")
%     p_indexPathLength_byMap = anova1([visIndexPathLength; invisIndexPathLength]); close; close;
%     p_thumbPathLength_byMap = anova1([visThumbPathLength; invisThumbPathLength]); %close; close;
%     p_boxPathLength_byMap = anova1([visBoxPathLength; invisBoxPathLength]); close; close;
%     p_completionTime_byMap = anova1([visCompletionTime; invisCompletionTime]); close; close;
%     p_indexShear_byMap = anova1([visIndexShearMean; invisIndexShearMean]); close; close;
%     p_indexNormal_byMap = anova1([visIndexNormalMean; invisIndexNormalMean]) ; close; close;
%     p_thumbShear_byMap = anova1([visThumbShearMean; invisThumbShearMean]); close; close;
%     p_thumbNormal_byMap = anova1([visThumbNormalMean; invisThumbNormalMean]); close; close;
%     
%     %Check if any of the p-values are less than 0.05
%     isSignificantByMap = [p_indexPathLength_byMap;
%         p_thumbPathLength_byMap;
%         p_boxPathLength_byMap;
%         p_completionTime_byMap;
%         p_indexShear_byMap;
%         p_indexNormal_byMap;
%         p_thumbShear_byMap;
%         p_thumbNormal_byMap]
%     
%     disp("ANOVA by Cube Visibility Condition")
%     p_indexPathLength_byCond = anova1([visIndexPathLength; invisIndexPathLength]'); close; close;
%     p_thumbPathLength_byCond = anova1([visThumbPathLength; invisThumbPathLength]'); close; close;
%     p_boxPathLength_byCond = anova1([visBoxPathLength; invisBoxPathLength]'); close; close;
%     p_completionTime_byCond = anova1([visCompletionTime; invisCompletionTime]'); close; close;
%     p_indexShear_byCond = anova1([visIndexShearMean; invisIndexShearMean]'); close; close;
%     p_indexNormal_byCond = anova1([visIndexNormalMean; invisIndexNormalMean]'); close; close;
%     p_thumbShear_byCond = anova1([visThumbShearMean; invisThumbShearMean]'); close; close;
%     p_thumbNormal_byCond = anova1([visThumbNormalMean; invisThumbNormalMean]'); close; close;
%     
%     %Check if any of the p-values are less than 0.05
%     isSignificantByCond = [p_indexPathLength_byCond;
%         p_thumbPathLength_byCond;
%         p_boxPathLength_byCond;
%         p_completionTime_byCond;
%         p_indexShear_byCond;
%         p_indexNormal_byCond;
%         p_thumbShear_byCond;
%         p_thumbNormal_byCond]

%% Statistcal Tests v2 -- N-Way ANOVA
    % 2-way ANOVA
    % p = anovan(y, group)
    format short
    % Groups:
    %Mapping group:
    mappings(1:200,1) = {'1'};
    mappings(201:400,1) = {'2'};
    mappings(401:600,1) = {'3'};
    mappings(601:800,1) = {'4'};
    mappings(801:1000,1) = {'5'};
    
    %Visibility condition
    visibiltyCondition(1:100,1) = {'visibleCube'};
    visibiltyCondition(101:200,1) = {'invisibleCube'};
    visibiltyCondition(201:300,1) = {'visibleCube'};
    visibiltyCondition(301:400,1) = {'invisibleCube'};
    visibiltyCondition(401:500,1) = {'visibleCube'};
    visibiltyCondition(501:600,1) = {'invisibleCube'};
    visibiltyCondition(601:700,1) = {'visibleCube'};
    visibiltyCondition(701:800,1) = {'invisibleCube'};
    visibiltyCondition(801:900,1) = {'visibleCube'};
    visibiltyCondition(901:1000,1) = {'invisibleCube'};
    
    %Convert matrix of subject results to column vectors
    %CompletionTime Mapping 1 visCube group
    yCT_visMap1 = reshape(completionTimeMapping1(:,1:10),[],1);
    %CompletionTime Mapping 1 invisCube group
    yCT_invisMap1 = reshape(completionTimeMapping1(:,11:20),[],1);
    %CompletionTime Mapping 2 visCube group
    yCT_visMap2 = reshape(completionTimeMapping2(:,1:10),[],1);
    %CompletionTime Mapping 2 invisCube group
    yCT_invisMap2 = reshape(completionTimeMapping2(:,11:20),[],1);
    %CompletionTime Mapping 3 visCube group
    yCT_visMap3 = reshape(completionTimeMapping3(:,1:10),[],1);
    %CompletionTime Mapping 3 invisCube group
    yCT_invisMap3 = reshape(completionTimeMapping3(:,11:20),[],1);
    %CompletionTime Mapping 4 visCube group
    yCT_visMap4 = reshape(completionTimeMapping4(:,1:10),[],1);
    %CompletionTime Mapping 4 invisCube group
    yCT_invisMap4 = reshape(completionTimeMapping4(:,11:20),[],1);
    %CompletionTime Mapping 5 visCube group
    yCT_visMap5 = reshape(completionTimeMapping5(:,1:10),[],1);
    %CompletionTime Mapping 5 invisCube group
    yCT_invisMap5 = reshape(completionTimeMapping5(:,11:20),[],1);
    
    %IndexPathLength Mapping 1 visCube group
    yIPL_visMap1 = reshape(indexPathLengthMapping1(:,1:10),[],1);
    %IndexPathLength Mapping 1 invisCube group
    yIPL_invisMap1 = reshape(indexPathLengthMapping1(:,11:20),[],1);
    %IndexPathLength Mapping 2 visCube group
    yIPL_visMap2 = reshape(indexPathLengthMapping2(:,1:10),[],1);
    %IndexPathLength Mapping 2 invisCube group
    yIPL_invisMap2 = reshape(indexPathLengthMapping2(:,11:20),[],1);
    %IndexPathLength Mapping 3 visCube group
    yIPL_visMap3 = reshape(indexPathLengthMapping3(:,1:10),[],1);
    %IndexPathLength Mapping 3 invisCube group
    yIPL_invisMap3 = reshape(indexPathLengthMapping3(:,11:20),[],1);
    %IndexPathLength Mapping 4 visCube group
    yIPL_visMap4 = reshape(indexPathLengthMapping4(:,1:10),[],1);
    %IndexPathLength Mapping 4 invisCube group
    yIPL_invisMap4 = reshape(indexPathLengthMapping4(:,11:20),[],1);
    %IndexPathLength Mapping 5 visCube group
    yIPL_visMap5 = reshape(indexPathLengthMapping5(:,1:10),[],1);
    %IndexPathLength Mapping 5 invisCube group
    yIPL_invisMap5 = reshape(indexPathLengthMapping5(:,11:20),[],1);
    
    %ThumbPathLength Mapping 1 visCube group
    yTPL_visMap1 = reshape(thumbPathLengthMapping1(:,1:10),[],1);
    %ThumbPathLength Mapping 1 invisCube group
    yTPL_invisMap1 = reshape(thumbPathLengthMapping1(:,11:20),[],1);
    %ThumbPathLength Mapping 2 visCube group
    yTPL_visMap2 = reshape(thumbPathLengthMapping2(:,1:10),[],1);
    %ThumbPathLength Mapping 2 invisCube group
    yTPL_invisMap2 = reshape(thumbPathLengthMapping2(:,11:20),[],1);
    %ThumbPathLength Mapping 3 visCube group
    yTPL_visMap3 = reshape(thumbPathLengthMapping3(:,1:10),[],1);
    %ThumbPathLength Mapping 3 invisCube group
    yTPL_invisMap3 = reshape(thumbPathLengthMapping3(:,11:20),[],1);
    %ThumbPathLength Mapping 4 visCube group
    yTPL_visMap4 = reshape(thumbPathLengthMapping4(:,1:10),[],1);
    %ThumbPathLength Mapping 4 invisCube group
    yTPL_invisMap4 = reshape(thumbPathLengthMapping4(:,11:20),[],1);
    %ThumbPathLength Mapping 5 visCube group
    yTPL_visMap5 = reshape(thumbPathLengthMapping5(:,1:10),[],1);
    %ThumbPathLength Mapping 5 invisCube group
    yTPL_invisMap5 = reshape(thumbPathLengthMapping5(:,11:20),[],1);
    
    %BoxPathLength Mapping 1 visCube group
    yBPL_visMap1 = reshape(boxPathLengthMapping1(:,1:10),[],1);
    %BoxPathLength Mapping 1 invisCube group
    yBPL_invisMap1 = reshape(boxPathLengthMapping1(:,11:20),[],1);
    %BoxPathLength Mapping 2 visCube group
    yBPL_visMap2 = reshape(boxPathLengthMapping2(:,1:10),[],1);
    %BoxPathLength Mapping 2 invisCube group
    yBPL_invisMap2 = reshape(boxPathLengthMapping2(:,11:20),[],1);
    %BoxPathLength Mapping 3 visCube group
    yBPL_visMap3 = reshape(boxPathLengthMapping3(:,1:10),[],1);
    %BoxPathLength Mapping 3 invisCube group
    yBPL_invisMap3 = reshape(boxPathLengthMapping3(:,11:20),[],1);
    %BoxPathLength Mapping 4 visCube group
    yBPL_visMap4 = reshape(boxPathLengthMapping4(:,1:10),[],1);
    %BoxPathLength Mapping 4 invisCube group
    yBPL_invisMap4 = reshape(boxPathLengthMapping4(:,11:20),[],1);
    %BoxPathLength Mapping 5 visCube group
    yBPL_visMap5 = reshape(boxPathLengthMapping5(:,1:10),[],1);
    %BoxPathLength Mapping 5 invisCube group
    yBPL_invisMap5 = reshape(boxPathLengthMapping5(:,11:20),[],1);
    
    %IndexShearForceMag Mapping 1 visCube group
    yISF_visMap1 = reshape(meanIndexShearForceMapping1(:,1:10),[],1);
    %IndexShearForceMag Mapping 1 invisCube group
    yISF_invisMap1 = reshape(meanIndexShearForceMapping1(:,11:20),[],1);
    %IndexShearForceMag Mapping 2 visCube group
    yISF_visMap2 = reshape(meanIndexShearForceMapping2(:,1:10),[],1);
    %IndexShearForceMag Mapping 2 invisCube group
    yISF_invisMap2 = reshape(meanIndexShearForceMapping2(:,11:20),[],1);
    %IndexShearForceMag Mapping 3 visCube group
    yISF_visMap3 = reshape(meanIndexShearForceMapping3(:,1:10),[],1);
    %IndexShearForceMag Mapping 3 invisCube group
    yISF_invisMap3 = reshape(meanIndexShearForceMapping3(:,11:20),[],1);
    %IndexShearForceMag Mapping 4 visCube group
    yISF_visMap4 = reshape(meanIndexShearForceMapping4(:,1:10),[],1);
    %IndexShearForceMag Mapping 4 invisCube group
    yISF_invisMap4 = reshape(meanIndexShearForceMapping4(:,11:20),[],1);
    %IndexShearForceMag Mapping 5 visCube group
    yISF_visMap5 = reshape(meanIndexShearForceMapping5(:,1:10),[],1);
    %IndexShearForceMag Mapping 5 invisCube group
    yISF_invisMap5 = reshape(meanIndexShearForceMapping5(:,11:20),[],1);
    
    %IndexNormalForceMag Mapping 1 visCube group
    yINF_visMap1 = reshape(meanIndexNormalForceMapping1(:,1:10),[],1);
    %IndexNormalForceMag Mapping 1 invisCube group
    yINF_invisMap1 = reshape(meanIndexNormalForceMapping1(:,11:20),[],1);
    %IndexNormalForceMag Mapping 2 visCube group
    yINF_visMap2 = reshape(meanIndexNormalForceMapping2(:,1:10),[],1);
    %IndexNormalForceMag Mapping 2 invisCube group
    yINF_invisMap2 = reshape(meanIndexNormalForceMapping2(:,11:20),[],1);
    %IndexNormalForceMag Mapping 3 visCube group
    yINF_visMap3 = reshape(meanIndexNormalForceMapping3(:,1:10),[],1);
    %IndexNormalForceMag Mapping 3 invisCube group
    yINF_invisMap3 = reshape(meanIndexNormalForceMapping3(:,11:20),[],1);
    %IndexNormalForceMag Mapping 4 visCube group
    yINF_visMap4 = reshape(meanIndexNormalForceMapping4(:,1:10),[],1);
    %IndexNormalForceMag Mapping 4 invisCube group
    yINF_invisMap4 = reshape(meanIndexNormalForceMapping4(:,11:20),[],1);
    %IndexNormalForceMag Mapping 5 visCube group
    yINF_visMap5 = reshape(meanIndexNormalForceMapping5(:,1:10),[],1);
    %IndexNormalForceMag Mapping 5 invisCube group
    yINF_invisMap5 = reshape(meanIndexNormalForceMapping5(:,11:20),[],1);
    
    %ThumbShearForceMag Mapping 1 visCube group
    yTSF_visMap1 = reshape(meanThumbShearForceMapping1(:,1:10),[],1);
    %ThumbShearForceMag Mapping 1 invisCube group
    yTSF_invisMap1 = reshape(meanThumbShearForceMapping1(:,11:20),[],1);
    %ThumbShearForceMag Mapping 2 visCube group
    yTSF_visMap2 = reshape(meanThumbShearForceMapping2(:,1:10),[],1);
    %ThumbShearForceMag Mapping 2 invisCube group
    yTSF_invisMap2 = reshape(meanThumbShearForceMapping2(:,11:20),[],1);
    %ThumbShearForceMag Mapping 3 visCube group
    yTSF_visMap3 = reshape(meanThumbShearForceMapping3(:,1:10),[],1);
    %ThumbShearForceMag Mapping 3 invisCube group
    yTSF_invisMap3 = reshape(meanThumbShearForceMapping3(:,11:20),[],1);
    %ThumbShearForceMag Mapping 4 visCube group
    yTSF_visMap4 = reshape(meanThumbShearForceMapping4(:,1:10),[],1);
    %ThumbShearForceMag Mapping 4 invisCube group
    yTSF_invisMap4 = reshape(meanThumbShearForceMapping4(:,11:20),[],1);
    %ThumbShearForceMag Mapping 5 visCube group
    yTSF_visMap5 = reshape(meanThumbShearForceMapping5(:,1:10),[],1);
    %ThumbShearForceMag Mapping 5 invisCube group
    yTSF_invisMap5 = reshape(meanThumbShearForceMapping5(:,11:20),[],1);
    
    %ThumbNormalForceMag Mapping 1 visCube group
    yTNF_visMap1 = reshape(meanThumbNormalForceMapping1(:,1:10),[],1);
    %ThumbNormalForceMag Mapping 1 invisCube group
    yTNF_invisMap1 = reshape(meanThumbNormalForceMapping1(:,11:20),[],1);
    %ThumbNormalForceMag Mapping 2 visCube group
    yTNF_visMap2 = reshape(meanThumbNormalForceMapping2(:,1:10),[],1);
    %ThumbNormalForceMag Mapping 2 invisCube group
    yTNF_invisMap2 = reshape(meanThumbNormalForceMapping2(:,11:20),[],1);
    %ThumbNormalForceMag Mapping 3 visCube group
    yTNF_visMap3 = reshape(meanThumbNormalForceMapping3(:,1:10),[],1);
    %ThumbNormalForceMag Mapping 3 invisCube group
    yTNF_invisMap3 = reshape(meanThumbNormalForceMapping3(:,11:20),[],1);
    %ThumbNormalForceMag Mapping 4 visCube group
    yTNF_visMap4 = reshape(meanThumbNormalForceMapping4(:,1:10),[],1);
    %ThumbNormalForceMag Mapping 4 invisCube group
    yTNF_invisMap4 = reshape(meanThumbNormalForceMapping4(:,11:20),[],1);
    %ThumbNormalForceMag Mapping 5 visCube group
    yTNF_visMap5 = reshape(meanThumbNormalForceMapping5(:,1:10),[],1);
    %ThumbNormalForceMag Mapping 5 invisCube group
    yTNF_invisMap5 = reshape(meanThumbNormalForceMapping5(:,11:20),[],1);
    
    %BoxDrops Mapping 1 visCube group
    yBD_visMap1 = reshape(boxDropsMapping1(:,1:10),[],1);
    %BoxDrops Mapping 1 invisCube group
    yBD_invisMap1 = reshape(boxDropsMapping1(:,11:20),[],1);
    %BoxDrops Mapping 2 visCube group
    yBD_visMap2 = reshape(boxDropsMapping2(:,1:10),[],1);
    %BoxDrops Mapping 2 invisCube group
    yBD_invisMap2 = reshape(boxDropsMapping2(:,11:20),[],1);
    %BoxDrops Mapping 3 visCube group
    yBD_visMap3 = reshape(boxDropsMapping3(:,1:10),[],1);
    %BoxDrops Mapping 3 invisCube group
    yBD_invisMap3 = reshape(boxDropsMapping3(:,11:20),[],1);
    %BoxDrops Mapping 4 visCube group
    yBD_visMap4 = reshape(boxDropsMapping4(:,1:10),[],1);
    %BoxDrops Mapping 4 invisCube group
    yBD_invisMap4 = reshape(boxDropsMapping4(:,11:20),[],1);
    %BoxDrops Mapping 5 visCube group
    yBD_visMap5 = reshape(boxDropsMapping5(:,1:10),[],1);
    %BoxDrops Mapping 5 invisCube group
    yBD_invisMap5 = reshape(boxDropsMapping5(:,11:20),[],1);  
    
    %Find p-values
    %vertically concatenate columns of the same metric
    %Completion Time
    yCT = [yCT_visMap1; yCT_invisMap1; yCT_visMap2; yCT_invisMap2;...
        yCT_visMap3; yCT_invisMap3; yCT_visMap4; yCT_invisMap4;...
        yCT_visMap5; yCT_invisMap5];
    %     p_CompletionTime = anovan(yCT, {mappings, visibiltyCondition})
    [p_CompletionTime, tbl_CompletionTime , stats_CompletionTime ] = ...
        anovan(yCT,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_CompletionTime
    
    %Index Path Length
    yIPL = [yIPL_visMap1; yIPL_invisMap1; yIPL_visMap2; yIPL_invisMap2;...
        yIPL_visMap3; yIPL_invisMap3; yIPL_visMap4; yIPL_invisMap4;...
        yIPL_visMap5; yIPL_invisMap5];
    %     p_IndexPathLength = anovan(yIPL, {mappings, visibiltyCondition})
    [p_IndexPathLength, tbl_IndexPathLength, stats_IndexPathLength] = ...
        anovan(yIPL,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_IndexPathLength
    
    
    %Thumb Path Length
    yTPL = [yTPL_visMap1; yTPL_invisMap1; yTPL_visMap2; yTPL_invisMap2;...
        yTPL_visMap3; yTPL_invisMap3; yTPL_visMap4; yTPL_invisMap4;...
        yTPL_visMap5; yTPL_invisMap5];
    %     p_ThumbPathLength = anovan(yTPL, {mappings, visibiltyCondition})
    [p_ThumbPathLength, tbl_ThumbPathLength, stats_ThumbPathLength] = ...
        anovan(yTPL,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_ThumbPathLength
    
    
    %Box Path Length
    yBPL = [yBPL_visMap1; yBPL_invisMap1; yBPL_visMap2; yBPL_invisMap2;...
        yBPL_visMap3; yBPL_invisMap3; yBPL_visMap4; yBPL_invisMap4;...
        yBPL_visMap5; yBPL_invisMap5];
    %     p_BoxPathLength = anovan(yBPL, {mappings, visibiltyCondition})
    [p_BoxPathLength, tbl_BoxPathLength, stats_BoxPathLength] = ...
        anovan(yBPL,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_BoxPathLength
    
    %Index Shear Force
    yISF = [yISF_visMap1; yISF_invisMap1; yISF_visMap2; yISF_invisMap2;...
        yISF_visMap3; yISF_invisMap3; yISF_visMap4; yISF_invisMap4;...
        yISF_visMap5; yISF_invisMap5];
    %     p_IndexShearForce = anovan(yISF, {mappings, visibiltyCondition})
    [p_IndexShearForce, tbl_IndexShearForce, stats_IndexShearForce] = ...
        anovan(yISF,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_IndexShearForce
    
    
    %Index Normal Force
    yINF = [yINF_visMap1; yINF_invisMap1; yINF_visMap2; yINF_invisMap2;...
        yINF_visMap3; yINF_invisMap3; yINF_visMap4; yINF_invisMap4;...
        yINF_visMap5; yINF_invisMap5];
    %     p_IndexNormalForce = anovan(yINF, {mappings, visibiltyCondition})
    [p_IndexNormalForce, tbl_IndexNormalForce, stats_IndexNormalForce] = ...
        anovan(yINF,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_IndexNormalForce
    
    %Thumb Shear Force
    yTSF = [yTSF_visMap1; yTSF_invisMap1; yTSF_visMap2; yTSF_invisMap2;...
        yTSF_visMap3; yTSF_invisMap3; yTSF_visMap4; yTSF_invisMap4;...
        yTSF_visMap5; yTSF_invisMap5];
    %     p_ThumbShearForce = anovan(yTSF, {mappings, visibiltyCondition})
    [p_ThumbShearForce, tbl_ThumbShearForce, stats_ThumbShearForce] = ...
        anovan(yTSF,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_ThumbShearForce
    
    %Thumb Normal Force
    yTNF = [yTNF_visMap1; yTNF_invisMap1; yTNF_visMap2; yTNF_invisMap2;...
        yTNF_visMap3; yTNF_invisMap3; yTNF_visMap4; yTNF_invisMap4;...
        yTNF_visMap5; yTNF_invisMap5];
    %     p_ThumbNormalForce = anovan(yTNF, {mappings, visibiltyCondition})
    [p_ThumbNormalForce, tbl_ThumbNormalForce, stats_ThumbNormalForce] = ...
        anovan(yTNF,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_ThumbNormalForce
     
    
    %Box Drops
    yBD = [yBD_visMap1; yBD_invisMap1; yBD_visMap2; yBD_invisMap2;...
        yBD_visMap3; yBD_invisMap3; yBD_visMap4; yBD_invisMap4;...
        yBD_visMap5; yBD_invisMap5];
    %     p_BoxDrops = anovan(yBD, {mappings, visibiltyCondition})
    [p_BoxDrops tbl_BoxDrops , stats_BoxDrops ] = ...
        anovan(yCT,  {mappings, visibiltyCondition},...
        "Model","interaction", "Varnames",["mappings","visibiltyCondition"]);
    p_BoxDrops
    
    
end