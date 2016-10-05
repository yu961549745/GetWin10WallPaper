package org.yjt.win10pic;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * 选择横向壁纸
 * 
 * @author yjt
 */
public class WallPaperSelector {
	private int[] getJpgSize(InputStream in) throws Exception {
		// is JPEG file
		if (!(in.read() == 0xFF && in.read() == 0xD8)) {
			return null;
		}
		// find FFC0
		int p = 0, c;
		boolean finded = false;
		while ((c = in.read()) != -1) {
			if (p == 0xFF && c == 0xC0) {
				finded = true;
				break;
			}
			p = c;
		}
		if (!finded) {
			return null;
		}
		// read height and width
		in.skip(3);
		int h = (in.read() << 8) | in.read();
		int w = (in.read() << 8) | in.read();
		return new int[] { h, w };
	}

	public int[] getJpgSize(File f) throws Exception {
		InputStream in = new FileInputStream(f);
		int[] r = getJpgSize(in);
		in.close();
		return r;
	}

	public boolean isWallPaper(File f) {
		try {
			int[] r = getJpgSize(f);
			if (r == null) {
				return false;
			}
			return r[0] < r[1];
		} catch (Exception e) {
			return false;
		}
	}

	public List<File> getWallpapers(File dir) {
		File[] fs = dir.listFiles();
		List<File> pics = new ArrayList<File>();
		for (File f : fs) {
			if (isWallPaper(f)) {
				pics.add(f);
			}
		}
		return pics;
	}
}
