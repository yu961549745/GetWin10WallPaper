package org.yjt.win10pic;

import java.awt.Desktop;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

/**
 * 提取Win10的Windows聚焦的横向壁纸，添加到用户图片文件夹
 * 
 * @author yjt
 */
public class RunWin10PicAdder {

	public static void main(String[] args) throws Exception {
		// get user folder
		String userFolder = System.getenv("USERPROFILE");
		if (userFolder == null) {
			System.err.println("can't find user folder");
			System.exit(1);
		}
		// folder of win10 wall papers
		File picDir = new File(
				userFolder,
				"./AppData/Local/Packages/Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy/LocalState/Assets");
		// user's picture folder
		File outDir = new File(userFolder, "./Pictures/Saved Pictures");
		// get horizontal pictures
		List<File> newPics = new WallPaperSelector().getWallpapers(picDir);
		// get exist pictures
		List<File> oldPics = Arrays.asList(outDir.listFiles());
		// add new pictures identified by MD5
		List<File> addpics = getAddablePics(oldPics, newPics);
		// copy new pictures to destination folder
		int k = nextIndex(oldPics);
		System.out.printf("find \t\t%3d pics\n", newPics.size());
		System.out.printf("start at\t%03d.jpg\n", k);
		for (File f : addpics) {
			FileCopyer
					.copy(f, new File(outDir, String.format("%03d.jpg", k++)));
		}
		System.out.printf("add \t\t%3d pics\n", addpics.size());
		// open destination folder
		Desktop.getDesktop().open(outDir);
	}

	/**
	 * 寻找图片文件的下一个编号
	 * 
	 * @param fs
	 * @return
	 */
	public static int nextIndex(List<File> fs) {
		int id = 0;
		for (File f : fs) {
			String name = f.getName();
			if (name.matches("\\d+\\.jpg")) {
				int n = Integer.valueOf(f.getName().replaceAll("\\D", ""));
				if (n > id) {
					id = n;
				}
			}
		}
		return id + 1;
	}

	/**
	 * 按照MD5添加文件
	 * 
	 * @param oldpics
	 * @param newpics
	 * @return
	 * @throws Exception
	 */
	public static List<File> getAddablePics(List<File> oldpics,
			List<File> newpics) throws Exception {
		Set<String> md5List = new TreeSet<String>();
		List<File> addpics = new ArrayList<File>();
		for (File f : oldpics) {
			md5List.add(FileMD5Getter.getMD5(f));
		}
		for (File f : newpics) {
			if (!md5List.contains(FileMD5Getter.getMD5(f))) {
				addpics.add(f);
			}
		}
		return addpics;
	}
}
