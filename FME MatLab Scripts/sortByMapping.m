%** Sorts subjects form expriment by their mapping condition
%   Also assumes subjects are sorted columnwise:
%   [visibleCube | invisibleCube]
%   in the SubjectData cell

function sortedVariable = sortByMapping(parameter, mapping)

%Pull data from workspace
numSubjects = evalin('base','numSubjects');

for j = 1:numSubjects
    sortedVariable(:,j) = parameter(mapping(j,:),j);
end
end