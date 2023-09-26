%   Creates an combined and color-coded errorbar plot
%   Also assumes subjects are sorted columnwise:
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [h1, h2, h3, h4, h5, h6, h7, h8, h9] = createCombinedPathLengthsPlot(...
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

alphaVal = evalin('base', 'alphaVal');

dataSeparation = 0.3;

numCols = 3;

%Create the vectors for x-values on which the data will lie:
% Index training1
vec1 = (1:numMappings)-jitterVal;
% Thumb training 1
vec2 = (1:numMappings);
% Box Training 1
vec3 = (1:numMappings)+jitterVal;

% Index Training 2
vec4 = numMappings+(1:numMappings)-jitterVal+dataSeparation;
% Thumb training 2
vec5 = numMappings+(1:numMappings)+dataSeparation;
% Box Training2
vec6 = numMappings+(1:numMappings)+jitterVal+dataSeparation;

% Index Testing
vec7 = 2*numMappings+(1:numMappings)-jitterVal+2*dataSeparation;
% Thumb Testing
vec8 = 2*numMappings+(1:numMappings)+2*dataSeparation;
% Box Testing
vec9 = 2*numMappings+(1:numMappings)+jitterVal+2*dataSeparation;


% Shade only the backgrounds of training data
xT = 0.5*(vec6(end)+ vec7(1)); % X-vlaue of test-train border
minY = 0;
maxY = 3.25;
minX = 0;
v = [minX minY; xT minY; xT maxY; minX maxY];
f = [1 2 3 4];
shadeColor = [0.5 0.5 0.5];

patch('Faces',f,'Vertices',v,'FaceColor',num2str(shadeColor),...
    'EdgeColor',num2str(shadeColor),...
    'EdgeAlpha',alphaVal,'FaceAlpha',alphaVal); hold on;

% Section labels:
textYCoord = maxY-0.25;
text(0.5*(vec3(end)+vec4(1)), textYCoord,...
    "Training", "HorizontalAlignment","center", "FontSize", 20)
text(vec8(2), textYCoord,...
    "Testing", "HorizontalAlignment","center", "FontSize", 20)

% Data:

% Index training1
h1 = errorbar(vec1, ...
    indexP1.YData, indexP1.YNegativeDelta, indexP1.YPositiveDelta, ...
    "^", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
hold on;
% Thumb training 1
h2 = errorbar(vec2, ...
    thumbP1.YData, thumbP1.YNegativeDelta, thumbP1.YPositiveDelta, ...
    "o", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Box Training 1
h3 = errorbar(vec3,...
    boxP1.YData, boxP1.YNegativeDelta, boxP1.YPositiveDelta, ...
    "s", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Index Training 2
h4 = errorbar(vec4,...
    indexP2.YData, indexP2.YNegativeDelta, indexP2.YPositiveDelta, ...
    "^", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Thumb training 2
h5 = errorbar(vec5,...
    thumbP2.YData, thumbP2.YNegativeDelta, thumbP2.YPositiveDelta, ...
    "o", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Box Training2
h6 = errorbar(vec6,...
    boxP2.YData, boxP2.YNegativeDelta, boxP2.YPositiveDelta, ...
    "s", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Index Testing
h7 = errorbar(vec7,...
    indexP3.YData, indexP3.YNegativeDelta, indexP3.YPositiveDelta, ...
    "^", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Thumb Testing
h8 = errorbar(vec8, ...
    thumbP3.YData, thumbP3.YNegativeDelta, thumbP3.YPositiveDelta, ...
    "o", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);
% Box Testing
h9 = errorbar(vec9, ...
    boxP3.YData, boxP3.YNegativeDelta, boxP3.YPositiveDelta, ...
    "s", "MarkerFaceColor", str2num(trainingMap5Color), ...
    "Color", str2num(trainingMap5Color), "MarkerSize", markerSize, ...
    "LineWidth", 2);

% Line to separate training1 and training 2 plots
% xline((vec3(end)+ vec4(1))/2)

% Line to separate training 2 and testing plots
% xline((vec6(end)+ vec7(1))/2)


xTickVals = ...
    [0,...
    1:numMappings,...
    ((numMappings+1):2*numMappings)+dataSeparation,...
    (2*numMappings+1:3*numMappings)+2*dataSeparation];

%Plot Details
n=numCols;
minX = (max(xTickVals)+2*dataSeparation)-vec9(end);
xlim([minX max(xTickVals)+2*dataSeparation]);

tickLabels = [" ", repmat(["Dual Tactor", "Single Tactor", "Control"], 1, n)];
xticks(xTickVals);
set(gca, 'xTick', xTickVals, 'xticklabel', tickLabels);

xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);

ylim([minY maxY])
hold off;

end


