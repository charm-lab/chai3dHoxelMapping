%   Creates an combined and color-coded errorbar plot
%   Also assumes subjects are sorted columnwise:
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [h1, h2, h3] = createPathLengthsPlotTestPhase(...
    indexP3, thumbP3, boxP3, plotTitle, xAxisLabel, yAxisLabel)

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

showInidividualSubjects = evalin('base', 'showInidividualSubjects');

%Create the vectors for x-values on which the data will lie:
% Index training1
vec1 = (1:numMappings)-jitterVal;
% Thumb training 1
vec2 = (1:numMappings);
% Box Training 1
vec3 = (1:numMappings)+jitterVal;

% Single-color Plot
% % Data:
% % Index Testing
% h1 = errorbar(vec1,...
%     indexP3.YData, indexP3.YNegativeDelta, indexP3.YPositiveDelta, ...
%     "^", "MarkerFaceColor", str2num(testingMap5Color), ...
%     "Color", str2num(testingMap5Color), "MarkerSize", markerSize, ...
%     "LineWidth", 2); hold on;
% % Thumb Testing
% h2 = errorbar(vec2, ...
%     thumbP3.YData, thumbP3.YNegativeDelta, thumbP3.YPositiveDelta, ...
%     "o", "MarkerFaceColor", str2num(testingMap5Color), ...
%     "Color", str2num(testingMap5Color), "MarkerSize", markerSize, ...
%     "LineWidth", 2);
% % Box Testing
% h3 = errorbar(vec3, ...
%     boxP3.YData, boxP3.YNegativeDelta, boxP3.YPositiveDelta, ...
%     "s", "MarkerFaceColor", str2num(testingMap5Color), ...
%     "Color", str2num(testingMap5Color), "MarkerSize", markerSize, ...
%     "LineWidth", 2);

% Multi-Color Plot:
colors  = [testingMap1Color testingMap3Color testingMap5Color];
% Data:
% Index Testing
for i =1:numMappings
%h1 = 
errorbar(vec1(i),...
    indexP3.YData(i), indexP3.YNegativeDelta(i), indexP3.YPositiveDelta(i), ...
    "s", "MarkerFaceColor", "w", "Color", colors(i), ...
    "MarkerSize", markerSize, "LineWidth", 2); hold on;
    
% Thumb Testing
%h2 = 
errorbar(vec2(i), ...
    thumbP3.YData(i), thumbP3.YNegativeDelta(i), thumbP3.YPositiveDelta(i), ...
    "^", "MarkerFaceColor", colors(i), "Color", str2num(colors(i)), ...
    "MarkerSize", markerSize, "LineWidth", 2);

% Box Testing
%h3 = 
errorbar(vec3(i), ...
    boxP3.YData(i), boxP3.YNegativeDelta(i), boxP3.YPositiveDelta(i), ...
    "s", "MarkerFaceColor", colors(i), "Color", colors(i), ...
    "MarkerSize", markerSize, "LineWidth", 2);
end

% xTickVals = [0, 1:numMappings];

% Plot Details
xlim([0.5 numMappings+0.5]);
xticks(1:numMappings);
tickLabels = ["Dual Tactor", "Single Tactor", "Control"];
set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);

% Create dummy points in black color to be cut off intentionally so legend
% icons appear in correct color
h1 = errorbar(0, 0, 0, 0, "s", "MarkerFaceColor","w", ...
    "Color", "k", "MarkerSize", markerSize, "LineWidth", 2);
h2 = errorbar(0, 0, 0, 0, "^", "MarkerFaceColor","k", ...
    "Color", "k", "MarkerSize", markerSize, "LineWidth", 2);
h3 = errorbar(0, 0, 0, 0, "s", "MarkerFaceColor","k", ...
    "Color", "k", "MarkerSize", markerSize, "LineWidth", 2);

% title(plotTitle);
hold off;

ylim([0.99 1.35])

% improvePlot_v2(false, true, 22, 1200, 600);
hold off;

end


