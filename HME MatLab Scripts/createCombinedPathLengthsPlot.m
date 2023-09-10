%   Creates an combined and color-coded errorbar plot
%   Also assumes subjects are sorted columnwise:
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [combinedPathLengthsPlot] = createCombinedPathLengthsPlot(...
    indexP1,indexP2,indexP3,...
    thumbP1,thumbP2,thumbP3, ...
    boxP1,boxP2,boxP3,...
    plotTitle, xAxisLabel, yAxisLabel)

%Pull variables from workspace
trainingMap1Color = evalin('base','trainingMap1Color');
testingMap1Color = evalin('base','testingMap1Color');
trainingMap3Color = evalin('base','trainingMap3Color');
testingMap3Color = evalin('base','testingMap3Color');
trainingMap5Color = evalin('base','trainingMap5Color');
testingMap5Color = evalin('base','testingMap5Color');

boxTrainingColor = evalin('base','boxTrainingColor');
boxTestingColor = evalin('base','boxTestingColor');

jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');

markerSize = evalin('base', 'markerSize');

dataSeparation = 0.4;

numCols = 3;

% Index training1
vec1 = (1:numMappings)-jitterVal;
h1 = errorbar(vec1, ...
    indexP1.YData, indexP1.YNegativeDelta, indexP1.YPositiveDelta, ...
    "*", "MarkerFaceColor", str2num(trainingMap1Color), ...
    "Color", str2num(trainingMap1Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
hold on;
% Thumb training 1
vec2 = (1:numMappings)+jitterVal;
h2 = errorbar(vec2, ...
    thumbP1.YData, thumbP1.YNegativeDelta, thumbP1.YPositiveDelta, ...
    "o", "MarkerFaceColor", str2num(trainingMap1Color), ...
    "Color", str2num(trainingMap1Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Box Training 1
vec3 = (1:numMappings)+jitterVal*2;
h3 = errorbar(vec3,...
    boxP1.YData, boxP1.YNegativeDelta, boxP1.YPositiveDelta, ...
    "s", "MarkerFaceColor", str2num(trainingMap1Color), ...
    "Color", str2num(trainingMap1Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Index Training 2
vec4 = numMappings+(1:numMappings)-jitterVal+dataSeparation;
h4 = errorbar(vec4,...
    indexP2.YData, indexP2.YNegativeDelta, indexP2.YPositiveDelta, ...
    "*", "MarkerFaceColor", str2num(trainingMap3Color), ...
    "Color", str2num(trainingMap3Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Thumb training 2
vec5 = numMappings+(1:numMappings)+jitterVal+dataSeparation;
h5 = errorbar(vec5,...
    thumbP2.YData, thumbP2.YNegativeDelta, thumbP2.YPositiveDelta, ...
    "o", "MarkerFaceColor", str2num(trainingMap3Color), ...
    "Color", str2num(trainingMap3Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Box Training2
vec6 = numMappings+(1:numMappings)+2*jitterVal+dataSeparation;
h6 = errorbar(vec6,...
    boxP2.YData, boxP2.YNegativeDelta, boxP2.YPositiveDelta, ...
    "s", "MarkerFaceColor", str2num(trainingMap3Color), ...
    "Color", str2num(trainingMap3Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Index Testing
vec7 = 2*numMappings+(1:numMappings)-jitterVal+2*dataSeparation;
h7 = errorbar(vec7,...
    indexP3.YData, indexP3.YNegativeDelta, indexP3.YPositiveDelta, ...
    "*", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Thumb Testing
vec8 = 2*numMappings+(1:numMappings)+jitterVal+2*dataSeparation;
h8 = errorbar(vec8, ...
    thumbP3.YData, thumbP3.YNegativeDelta, thumbP3.YPositiveDelta, ...
    "o", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Box Testing
vec9 = 2*numMappings+(1:numMappings)+2*jitterVal+2*dataSeparation;
h9 = errorbar(vec9, ...
    boxP3.YData, boxP3.YNegativeDelta, boxP3.YPositiveDelta, ...
    "s", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Line to separate training1 and training 2 plots
xline((vec3(end)+ vec4(1))/2)

% Line to separate training 2 and testing plots
xline((vec6(end)+ vec7(1))/2)

legend([h1(1), h2(1), h3(1), h4(1), h5(1), h6(1), h7(1), h8(1), h9(1)],...
    "Index Testing 1", "Thumb Testing 1", "Cube Testing 1",...
    "Index Testing 2", "Thumb Testing 2", "Cube Testing 2",...
    "Index Training", "Thumb Training", "Cube Training",...
    "Location", "northoutside", "NumColumns",3);

xTickVals = ...
    [0,...
    1:numMappings,...
    ((numMappings+1):2*numMappings)+dataSeparation,...
    (2*numMappings+1:3*numMappings)+2*dataSeparation];

%Plot Details
n=numCols;
xlim([0 max(xTickVals)+2*dataSeparation]);

tickLabels = [" ", repmat(["Dual Tactor", "Single Tactor", "Control"], 1, n)];
xticks(xTickVals);
set(gca, 'xTick', xTickVals, 'xticklabel', tickLabels); %#ok<NBRAK>
% xticks(0:(6*n-n));
% set(gca,'xTick', [0:(6*n-n)],'xticklabel', tickLabels); %#ok<NBRAK>
% set(gca,'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);
hold off;

end


