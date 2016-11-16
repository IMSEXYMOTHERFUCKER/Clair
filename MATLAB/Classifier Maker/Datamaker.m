for i=1:753
   hand=imread(['PosBayesTr/p' int2str(i) '.jpg']);
   sz=size(hand);
   dataBayesTr(i)=struct('imageFilename',['PosBayesTr/p' int2str(i) '.jpg'],'objectBoundingBoxes',[1 1 sz(2) sz(1)]);
end