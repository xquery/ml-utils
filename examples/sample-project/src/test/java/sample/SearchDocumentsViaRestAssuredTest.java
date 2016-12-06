package sample;

import static com.jayway.restassured.RestAssured.basic;
import static com.jayway.restassured.RestAssured.get;

import org.junit.Before;
import org.junit.Test;

import com.jayway.restassured.RestAssured;
import com.jayway.restassured.path.json.JsonPath;
import com.marklogic.client.document.XMLDocumentManager;
import com.marklogic.client.helper.DatabaseClientConfig;
import com.marklogic.client.io.DocumentMetadataHandle;
import com.marklogic.client.io.Format;
import com.marklogic.client.io.StringHandle;
import com.marklogic.junit.Fragment;

/**
 * This test shows an example of using RestAssured instead of the MarkLogic Java API to invoke MarkLogic REST API
 * endpoints. The key is that we can easily configure RestAssured by reusing the configuration object retrieved from the
 * Spring container.
 */
public class SearchDocumentsViaRestAssuredTest extends AbstractSampleProjectTest {

    @Before
    public void initializeRestAssured() {
        logger.info("Initializing RestAssured...");

        String protocol = "http://";
        
        /*
         * If SSL were enabled on the REST API server, would need to uncomment these lines. 
         */
        // protocol = "https://";
        // RestAssured.config = RestAssured.config().sslConfig(
        // SSLConfig.sslConfig().allowAllHostnames().relaxedHTTPSValidation());

        DatabaseClientConfig config = getApplicationContext().getBean(DatabaseClientConfig.class);
        RestAssured.baseURI = protocol + config.getHost();
        RestAssured.port = config.getPort();
        RestAssured.authentication = basic(config.getUsername(), config.getPassword());
        

        logger.info("RestAssured URI: " + RestAssured.baseURI);
        logger.info("RestAssured port: " + RestAssured.port);
    }

    @Test
    public void twoDocuments() {
        loadPerson("/jane.xml", "Jane", "This is a sample document");
        loadPerson("/john.xml", "John", "This is another sample document");

        /**
         * This shows a simple RestAssured GET call that will return XML, and we can easily parse that to get a Fragment
         * instance that we can use for making assertions.
         */
        Fragment response = parse(get("/v1/search").asString());
        response.assertElementExists("/search:response/search:result[@uri = '/jane.xml']");
        response.assertElementExists("/search:response/search:result[@uri = '/john.xml']");

        /**
         * This shows searching for a term and then asserting on the total and snippet highlighting in the result.
         */
        response = parse(get("/v1/search?q=Jane").asString());
        response.assertElementExists("/search:response[@total = '1']/search:result[@uri = '/jane.xml']/search:snippet/search:match/search:highlight[. = 'Jane']");

        /**
         * RestAssured also makes it easy to assert on JSON responses.
         */
        JsonPath json = get("/v1/search?q=John&format=json").jsonPath();
        assertEquals(1, json.getInt("total"));
        assertEquals("/john.xml", json.get("results[0].uri"));
    }

    /**
     * This test verifies that the search options are loaded by ml-gradle for both the "main" REST server -
     * "sample-project" and the "test" REST server - "sample-project-test".
     */
    @Test
    public void searchWithSampleProjectOptions() {
        XMLDocumentManager mgr = getClient().newXMLDocumentManager();
        mgr.write("/doc1.xml", new DocumentMetadataHandle().withCollections("sample-project-docs"), new StringHandle(
                "<doc>This is the first doc</doc>").withFormat(Format.XML));
        mgr.write("/doc2.xml", new DocumentMetadataHandle().withCollections("some-other-docs"), new StringHandle(
                "<doc>This is the second doc</doc>").withFormat(Format.XML));

        Fragment response = parse(get("/v1/search?options=sample-project-options").asString());
        assertEquals(
                "Only doc1.xml should have been returned, since the options constrain to the collection that it's in",
                "1", response.getAttributeValue("/search:response", "total"));
        assertEquals("/doc1.xml", response.getAttributeValue("/search:response/search:result", "uri"));
    }
}
