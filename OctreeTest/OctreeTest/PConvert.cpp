//#include "PCconvert.h"
//
//
//
//PCconvert::PCconvert()
//{
//}
//
//
//PCconvert::~PCconvert()
//{
//}
//void PCconvert::prepare() {
//
//	// if sources contains directories, use files inside the directory instead
//	vector<string> sourceFiles;
//	for (const auto &source : sources) {
//		path pSource(source);
//		if (boost::filesystem::is_directory(pSource)) {
//			boost::filesystem::directory_iterator it(pSource);
//			for (; it != boost::filesystem::directory_iterator(); it++) {
//				path pDirectoryEntry = it->path();
//				if (boost::filesystem::is_regular_file(pDirectoryEntry)) {
//					string filepath = pDirectoryEntry.string();
//					if (boost::iends_with(filepath, ".las")
//						|| boost::iends_with(filepath, ".laz")
//						|| boost::iends_with(filepath, ".xyz")
//						|| boost::iends_with(filepath, ".pts")
//						|| boost::iends_with(filepath, ".ptx")
//						|| boost::iends_with(filepath, ".ply")) {
//						sourceFiles.push_back(filepath);
//					}
//				}
//			}
//		}
//		else if (boost::filesystem::is_regular_file(pSource)) {
//			sourceFiles.push_back(source);
//		}
//	}
//	this->sources = sourceFiles;
//
//	pointAttributes = PointAttributes();
//	pointAttributes.add(PointAttribute::POSITION_CARTESIAN);
//	for (const auto &attribute : outputAttributes) {
//		if (attribute == "RGB") {
//			pointAttributes.add(PointAttribute::COLOR_PACKED);
//		}
//		else if (attribute == "INTENSITY") {
//			pointAttributes.add(PointAttribute::INTENSITY);
//		}
//		else if (attribute == "CLASSIFICATION") {
//			pointAttributes.add(PointAttribute::CLASSIFICATION);
//		}
//		else if (attribute == "NORMAL") {
//			pointAttributes.add(PointAttribute::NORMAL_OCT16);
//		}
//	}
//}
//void PCconvert::convert() {
//	prepare();
//
//	long long pointsProcessed = 0;
//
//	AABB aabb = calculateAABB();
//	cout << "AABB: " << endl << aabb << endl;
//	aabb.makeCubic();
//	cout << "cubic AABB: " << endl << aabb << endl;
//
//	if (diagonalFraction != 0) {
//		spacing = (float)(aabb.size.length() / diagonalFraction);
//		cout << "spacing calculated from diagonal: " << spacing << endl;
//	}
//
//	if (pageName.size() > 0) {
//		generatePage(pageName);
//		workDir = workDir + "/pointclouds/" + pageName;
//	}
//
//	PotreeWriter *writer = NULL;
//	if (fs::exists(fs::path(this->workDir + "/cloud.js"))) {
//
//		if (storeOption == StoreOption::ABORT_IF_EXISTS) {
//			cout << "ABORTING CONVERSION: target already exists: " << this->workDir << "/cloud.js" << endl;
//			cout << "If you want to overwrite the existing conversion, specify --overwrite" << endl;
//			cout << "If you want add new points to the existing conversion, make sure the new points ";
//			cout << "are contained within the bounding box of the existing conversion and then specify --incremental" << endl;
//
//			return;
//		}
//		else if (storeOption == StoreOption::OVERWRITE) {
//			fs::remove_all(workDir + "/data");
//			fs::remove_all(workDir + "/temp");
//			fs::remove(workDir + "/cloud.js");
//			writer = new PotreeWriter(this->workDir, aabb, spacing, maxDepth, scale, outputFormat, pointAttributes, quality);
//			writer->setProjection(this->projection);
//		}
//		else if (storeOption == StoreOption::INCREMENTAL) {
//			writer = new PotreeWriter(this->workDir, quality);
//			writer->loadStateFromDisk();
//		}
//	}
//	else {
//		writer = new PotreeWriter(this->workDir, aabb, spacing, maxDepth, scale, outputFormat, pointAttributes, quality);
//		writer->setProjection(this->projection);
//	}
//
//	if (writer == NULL) {
//		return;
//	}
//
//	vector<AABB> boundingBoxes;
//	vector<int> numPoints;
//	vector<string> sourceFilenames;
//
//	for (const auto &source : sources) {
//		cout << "READING:  " << source << endl;
//
//		PointReader *reader = createPointReader(source, pointAttributes);
//
//		boundingBoxes.push_back(reader->getAABB());
//		numPoints.push_back(reader->numPoints());
//		sourceFilenames.push_back(fs::path(source).filename().string());
//
//		writeSources(this->workDir, sourceFilenames, numPoints, boundingBoxes, this->projection);
//		if (this->sourceListingOnly) {
//			reader->close();
//			delete reader;
//
//			continue;
//		}
//
//		while (reader->readNextPoint()) {
//			pointsProcessed++;
//
//			Point p = reader->getPoint();
//			writer->add(p);
//
//			if ((pointsProcessed % (1'000'000)) == 0) {
//				writer->processStore();
//				writer->waitUntilProcessed();
//
//				auto end = high_resolution_clock::now();
//				long long duration = duration_cast<milliseconds>(end - start).count();
//				float seconds = duration / 1'000.0f;
//
//				stringstream ssMessage;
//
//				ssMessage.imbue(std::locale(""));
//				ssMessage << "INDEXING: ";
//				ssMessage << pointsProcessed << " points processed; ";
//				ssMessage << writer->numAccepted << " points written; ";
//				ssMessage << seconds << " seconds passed";
//
//				cout << ssMessage.str() << endl;
//			}
//			if ((pointsProcessed % (10'000'000)) == 0) {
//				cout << "FLUSHING: ";
//
//				auto start = high_resolution_clock::now();
//
//				writer->flush();
//
//				auto end = high_resolution_clock::now();
//				long long duration = duration_cast<milliseconds>(end - start).count();
//				float seconds = duration / 1'000.0f;
//
//				cout << seconds << "s" << endl;
//			}
//
//			//if(pointsProcessed >= 10'000'000){
//			//	break;
//			//}
//		}
//		reader->close();
//		delete reader;
//
//
//	}
//
//	cout << "closing writer" << endl;
//	writer->flush();
//	writer->close();
//
//	writeSources(this->workDir + "/sources.json", sourceFilenames, numPoints, boundingBoxes, this->projection);
//
//	float percent = (float)writer->numAccepted / (float)pointsProcessed;
//	percent = percent * 100;
//
//	auto end = high_resolution_clock::now();
//	long long duration = duration_cast<milliseconds>(end - start).count();
//
//
//	cout << endl;
//	cout << "conversion finished" << endl;
//	cout << pointsProcessed << " points were processed and " << writer->numAccepted << " points ( " << percent << "% ) were written to the output. " << endl;
//
//	cout << "duration: " << (duration / 1000.0f) << "s" << endl;
//
//}