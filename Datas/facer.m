folders=dir('lfw');
for i=10:5751
   files=dir(['lfw/' folders(i).name]);
   mat=imread(['lfw/' folders(i).name '/' files(3).name]);
   imwrite(mat,['Faces/' int2str(i-9) '.jpg']);
end